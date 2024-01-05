// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActActionEffect.h"
#include "ActActionEffect_Thorns.generated.h"

class UActAttributeComponent;
/**
 * 
 */
UCLASS()
class UEACTIONROGUELIKE_API UActActionEffect_Thorns : public UActActionEffect
{
	GENERATED_BODY()

public:

	UActActionEffect_Thorns();
	
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

	UPROPERTY(EditDefaultsOnly)
	float PercentageReflected;
	
protected:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta, float ActualDelta);
};
