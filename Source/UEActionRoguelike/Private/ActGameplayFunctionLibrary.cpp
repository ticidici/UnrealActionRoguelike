// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameplayFunctionLibrary.h"

#include "ActAttributeComponent.h"

bool UActGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(TargetActor);
	if(AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, DamageAmount);
	}
	return false;
}

bool UActGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	bool Result = ApplyDamage(DamageCauser, TargetActor, DamageAmount);
	//if we don't put the impulse inside the if it moves even in death
	UPrimitiveComponent* HitComp = HitResult.GetComponent();
	if(HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
	{
		HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 4500.f, HitResult.ImpactPoint, HitResult.BoneName);
	}
	return Result;
}
