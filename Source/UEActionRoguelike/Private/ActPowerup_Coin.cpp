// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPowerup_Coin.h"

#include "ActPlayerState.h"

AActPowerup_Coin::AActPowerup_Coin()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void AActPowerup_Coin::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	AActPlayerState* PlayerState = AActPlayerState::GetActPlayerState(Cast<APlayerController>(InstigatorPawn->GetController()));
	if(PlayerState)
	{
		PlayerState->VaryCredits(+50);
		HideAndCooldownPowerup(RespawnTime);
	}
}