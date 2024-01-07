// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPowerup_GrantAction.h"

#include "ActActionComponent.h"

AActPowerup_GrantAction::AActPowerup_GrantAction()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
	RespawnTime = -1;
}

void AActPowerup_GrantAction::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}

	UActActionComponent* ActionComp = InstigatorPawn->FindComponentByClass<UActActionComponent>();
	
	if(ActionComp)
	{
		ActionComp->AddAction(this, ActionClassToGrant);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Action Granted: " + ActionClassToGrant->GetName());
		HideAndCooldownPowerup(RespawnTime);
	}
}
