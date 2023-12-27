// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActPowerupActor.h"
#include "ActPowerup_Coin.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONROGUELIKE_API AActPowerup_Coin : public AActPowerupActor
{
	GENERATED_BODY()
	
public:
	
	AActPowerup_Coin();
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Components");
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
