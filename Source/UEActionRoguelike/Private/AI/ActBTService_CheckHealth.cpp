// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActBTService_CheckHealth.h"

#include "ActAttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UActBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ensure(AIPawn))
	{
		UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(AIPawn);
		if(ensure(AttributeComp))
		{
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, AttributeComp->isLowHealth());
		}
	}
}
