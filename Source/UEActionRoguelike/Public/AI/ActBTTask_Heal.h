// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ActBTTask_Heal.generated.h"

/**
 * 
 */
UCLASS()
class UEACTIONROGUELIKE_API UActBTTask_Heal : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UActBTTask_Heal();
	
};
