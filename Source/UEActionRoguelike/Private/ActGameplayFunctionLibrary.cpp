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

//Pass a modified Hit Result (impact normal) if we want a custom impulse
bool UActGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult, bool bDefaultImpulse)
{
	bool Result = ApplyDamage(DamageCauser, TargetActor, DamageAmount);
	//if we don't put the impulse inside the if it moves even in death
	UPrimitiveComponent* HitComp = HitResult.GetComponent();
	if(HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
	{
		FVector_NetQuantizeNormal Impulse = HitResult.ImpactNormal;
		if(bDefaultImpulse) Impulse = -HitResult.ImpactNormal * 5000.f;//default impulse gets the normal and turns it around
		
		HitComp->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint, HitResult.BoneName);
	}
	return Result;
}
