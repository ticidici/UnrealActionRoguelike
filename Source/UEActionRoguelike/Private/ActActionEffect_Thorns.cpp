// Fill out your copyright notice in the Description page of Project Settings.


#include "ActActionEffect_Thorns.h"

#include "ActActionComponent.h"
#include "ActAttributeComponent.h"

UActActionEffect_Thorns::UActActionEffect_Thorns()
{
	Duration = 0;//infinite
	Period = 0;//no periodic effect

	PercentageReflected = 20;
	ActionName = "Thorns";
}

void UActActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	AActor* Owner = GetOwningComponent()->GetOwner();
	if(Owner)
	{
		UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(Owner);
		AttributeComp->OnHealthChanged.AddDynamic(this, &UActActionEffect_Thorns::OnHealthChanged);
	}
}

void UActActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	AActor* Owner = GetOwningComponent()->GetOwner();
	if(Owner)
	{
		UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(Owner);
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UActActionEffect_Thorns::OnHealthChanged);
	}
}

void UActActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta, float ActualDelta)
{
	//if dead, can't return damage
	if(NewHealth <= 0) return;
	
	UActAttributeComponent* InstigatorAttributeComp = UActAttributeComponent::GetAttributes(InstigatorActor);
	AActor* Owner = GetOwningComponent()->GetOwner();
	float ReflectedDamage = -FMath::CeilToInt32(FMath::Abs(ActualDelta) * PercentageReflected * 0.01f);
	
	//TODO pass gameplay tag, something like "Irreflectable", to avoid an infinite loop, also burning effect should have this
	InstigatorAttributeComp->ApplyHealthChange(Owner, ReflectedDamage);
}
