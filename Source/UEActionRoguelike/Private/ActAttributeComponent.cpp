// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAttributeComponent.h"

// Sets default values for this component's properties
UActAttributeComponent::UActAttributeComponent()
{
	MaxHealth = 120;
	Health = MaxHealth;
}

bool UActAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UActAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	Health = FMath::Clamp(Health, 0, MaxHealth);
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;//for now
}
