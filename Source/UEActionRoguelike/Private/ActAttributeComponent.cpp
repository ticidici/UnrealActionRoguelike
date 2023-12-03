// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

// Sets default values for this component's properties
UActAttributeComponent::UActAttributeComponent()
{
	MaxHealth = 999;
	Health = MaxHealth;
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

bool UActAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	
	Health += Delta;

	Health = FMath::Clamp(Health, 0, MaxHealth);

	float ActualDelta = Health - OldHealth;

	//float RandomDamage = FMath::Floor(FMath::RandRange(1.0, 100.0));
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return ActualDelta != 0;
}
