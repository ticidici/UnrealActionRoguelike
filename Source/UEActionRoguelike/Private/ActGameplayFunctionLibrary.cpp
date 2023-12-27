// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameplayFunctionLibrary.h"

#include "ActAttributeComponent.h"
#include "Blueprint/UserWidget.h"

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
	const FHitResult& HitResult, FVector CustomImpulse, bool bDefaultImpulse)
{
	bool Result = ApplyDamage(DamageCauser, TargetActor, DamageAmount);
	//if we don't put the impulse inside the if it moves even in death
	UPrimitiveComponent* HitComp = HitResult.GetComponent();
	if(HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
	{
		FVector Impulse = CustomImpulse;
		if(bDefaultImpulse)
		{
			Impulse = HitResult.TraceEnd - HitResult.TraceStart;
			Impulse.Normalize();
			Impulse *= 5000.f;
		}
		
		HitComp->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint, HitResult.BoneName);
	}
	return Result;
}
