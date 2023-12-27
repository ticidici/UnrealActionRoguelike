// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPowerup_HealthPotion.h"

#include "ActAttributeComponent.h"
#include "ActPlayerState.h"

AActPowerup_HealthPotion::AActPowerup_HealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void AActPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	
	UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(InstigatorPawn);
	if(ensure(AttributeComp))
	{
		//if full health or not enough credits
		AActPlayerState* PlayerState = AActPlayerState::GetActPlayerState(Cast<APlayerController>(InstigatorPawn->GetController()));

		if(AttributeComp->isFullHealth() || (PlayerState && PlayerState->GetCredits() < CreditsPrice))
		{
			//hide short time just to give some kind of feedback
			HideAndCooldownPowerup(1.0f);
		}
		else if(AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
		{
			//if there is no player state, healing is free (an enemy, for example)
			HideAndCooldownPowerup(RespawnTime);
			if(PlayerState)
			{
				PlayerState->VaryCredits(-CreditsPrice);
			}
		}
	}
}
