// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActAIController.h"

#include "AI/ActAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

void AActAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	
	/// we remove this to use a sensing component instead 
	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if(MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	// }
}
