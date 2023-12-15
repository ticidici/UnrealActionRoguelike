// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActAICharacter.h"

#include "ActAttributeComponent.h"
#include "ActWorldUserWidget.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

AActAICharacter::AActAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComp = CreateDefaultSubobject<UActAttributeComponent>("AttributeComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HitFlashTimeParamName = TEXT("TimeToHit");
	HitFlashSpeedParamName = TEXT("HitFlashSpeed");
	HitFlashColorParamName = TEXT("HitFlashColor");
}

void AActAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AActAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AActAICharacter::OnHealthChanged);
}

void AActAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = GetController<AAIController>();
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void AActAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void AActAICharacter::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta < 0.0f)
	{
		if(InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if(Delta > -AttributeComp->GetHealthMax() && ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UActWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		
		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashTimeParamName, GetWorld()->TimeSeconds);
		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashSpeedParamName, HitFlashSpeed);
		GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector(HitFlashColor.R, HitFlashColor.G, HitFlashColor.B));
		if(NewHealth <= 0.0f)
		{
			//stop bt
			AAIController* AIC = Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			
			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
