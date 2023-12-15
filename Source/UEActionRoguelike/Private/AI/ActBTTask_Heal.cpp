// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActBTTask_Heal.h"

#include "ActAttributeComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UActBTTask_Heal::UActBTTask_Heal()
{
	//Set defaults
}

EBTNodeResult::Type UActBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if(ensure(MyController))
	{
		ACharacter* MyCharacter = MyController->GetCharacter();
		if(MyCharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		AActor* ActorToHeal = MyCharacter;
		if(ActorToHeal == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(MyCharacter);

		if(AttributeComp == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		//TODO animation
		AttributeComp->ApplyHealthChange(MyCharacter, AttributeComp->GetHealthMax());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
