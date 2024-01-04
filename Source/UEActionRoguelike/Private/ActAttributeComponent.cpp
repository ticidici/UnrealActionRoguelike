// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

#include "ActGameModeBase.h"
#include "FunctionalTestingModule.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("act.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for AttributeComponent."), ECVF_Cheat);


// Sets default values for this component's properties
UActAttributeComponent::UActAttributeComponent()
{
	MaxHealth = 999;
	Health = MaxHealth;
	PercentageConsideredLowHealth = 25.0f;

	MaxRage = 500;
	Rage = 0;
	
	SetIsReplicatedByDefault(true);
}


bool UActAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool UActAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UActAttributeComponent::isFullHealth()
{
	return Health >= MaxHealth;
}

bool UActAttributeComponent::isLowHealth()
{
	return Health <= MaxHealth * PercentageConsideredLowHealth * 0.01f;
}

bool UActAttributeComponent::TrySpendRage(float Quantity)
{
	const float AbsQuantity = FMath::Abs(Quantity);
	if(AbsQuantity > Rage) return false;
	ApplyRageChange(-AbsQuantity);
	return true;
}

bool UActAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(Delta < 0 && !GetOwner()->CanBeDamaged())//found looking at cheat manager
	{
		UE_LOGFMT(LogTemp, Warning, "Can't be damaged!");
		return false;
	}

	if(Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	float OldHealth = Health;
	
	Health += Delta;

	Health = FMath::Clamp(Health, 0, MaxHealth);

	float ActualDelta = Health - OldHealth;

	//server will eventually call it when received the multicast
	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta, ActualDelta);
	MulticastHealthChanged(InstigatorActor, Health, Delta, ActualDelta);
	
	//Died
	if(ActualDelta < 0.0f && Health <= 0.0f)
	{
		AActGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AActGameModeBase>();
		if(GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

bool UActAttributeComponent::ApplyRageChange(float Delta)
{
	const float OriginalRage = Rage;
	Rage = FMath::Clamp(Rage + Delta, 0, MaxRage);
	float ActualDelta =  Rage - OriginalRage;
	OnRageChanged.Broadcast(this, Rage, Delta, ActualDelta);
	FString DebugMessage = "Rage = " + FString::SanitizeFloat(Rage);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, DebugMessage);
	return ActualDelta != 0;
}


UActAttributeComponent* UActAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return FromActor->FindComponentByClass<UActAttributeComponent>();
		//return Cast<UActAttributeComponent>(FromActor->GetComponentByClass(UActAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UActAttributeComponent::IsActorAlive(AActor* Actor)
{
	auto AttributeComp = GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

void UActAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
	float Delta, float ActualDelta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta, ActualDelta);
}

void UActAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActAttributeComponent, Health);
	DOREPLIFETIME(UActAttributeComponent, MaxHealth);
	DOREPLIFETIME(UActAttributeComponent, Rage);
	DOREPLIFETIME(UActAttributeComponent, MaxRage);
}
