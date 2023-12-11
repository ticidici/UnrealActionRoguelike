// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

#include "Logging/StructuredLog.h"


// Sets default values for this component's properties
UActAttributeComponent::UActAttributeComponent()
{
	MaxHealth = 999;
	Health = MaxHealth;
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

float UActAttributeComponent::GetHealthMax()
{
	return MaxHealth;
}

bool UActAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged())//found looking at cheat manager
	{
		UE_LOGFMT(LogTemp, Warning, "Can't be damaged!");
		return false;
	}
	
	float OldHealth = Health;
	
	Health += Delta;

	Health = FMath::Clamp(Health, 0, MaxHealth);

	float ActualDelta = Health - OldHealth;

	//float RandomDamage = FMath::Floor(FMath::RandRange(1.0, 100.0));
	
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
	
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
