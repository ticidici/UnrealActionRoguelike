// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActProjectileBase.h"
#include "ActDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONROGUELIKE_API AActDashProjectile : public AActProjectileBase
{
	GENERATED_BODY()

public:
	AActDashProjectile();
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	//Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	//Base class using BlueprintNativeEvent, we must override the "_Implementation" version
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;
};
