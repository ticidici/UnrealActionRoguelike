// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActPowerupActor.h"
#include "ActPowerup_GrantAction.generated.h"

class UActAction;
/**
 * 
 */
UCLASS()
class UEACTIONROGUELIKE_API AActPowerup_GrantAction : public AActPowerupActor
{
	GENERATED_BODY()
	
public:
	
	AActPowerup_GrantAction();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components");
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActAction> ActionClassToGrant;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
