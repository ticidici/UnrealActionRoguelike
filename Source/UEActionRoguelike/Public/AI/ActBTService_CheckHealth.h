// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ActBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONROGUELIKE_API UActBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowHealthKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector ActorToHealKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
