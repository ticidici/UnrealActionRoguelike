// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ActAICharacter.h"

#include "ActActionComponent.h"
#include "ActAttributeComponent.h"
#include "ActWorldUserWidget.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

AActAICharacter::AActAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComp = CreateDefaultSubobject<UActAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<UActActionComponent>("ActionComp");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
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

void AActAICharacter::BeginPlay()
{
	Super::BeginPlay();
	AAIController* AIC = GetController<AAIController>();
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsBool("CanHealSelf", bCanHealSelf);
	}
}

bool AActAICharacter::CanHealSelf() const
{
	return bCanHealSelf;
}

void AActAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = GetController<AAIController>();
	if(AIC)
	{
		if(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor") != NewTarget)
		{
			UActWorldUserWidget* PlayerSpottedWidget = CreateWidget<UActWorldUserWidget>(GetWorld(), PlayerSpottedWidgetClass);
			if(PlayerSpottedWidget)
			{
				PlayerSpottedWidget->AttachedActor = this;
				PlayerSpottedWidget->AddToViewport();
			}
			
			AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
		}
	}
}

void AActAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void AActAICharacter::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta, float ActualDelta, FGameplayTagContainer HealthVariationTags)
{
	if(ActualDelta < 0.0f)
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

		//Died
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
	else if(!AttributeComp->IsAlive())
	{
		//we reset lifespan to be able to keep hitting on the corpse, it only despawns when we leave it alone
		SetLifeSpan(10.0f);
	}

}
