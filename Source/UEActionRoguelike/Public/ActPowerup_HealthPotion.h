// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActPowerupActor.h"
#include "ActPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONROGUELIKE_API AActPowerup_HealthPotion : public AActPowerupActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components");
	UStaticMeshComponent* MeshComp;
	
public:

	AActPowerup_HealthPotion();
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
