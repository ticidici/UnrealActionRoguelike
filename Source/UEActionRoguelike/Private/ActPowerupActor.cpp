// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPowerupActor.h"

#include "Components/SphereComponent.h"


// Sets default values
AActPowerupActor::AActPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("PowerUp");
	RootComponent = SphereComp;

	RespawnTime = 10.0f;

	bReplicates = true;
}

void AActPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void AActPowerupActor::HideAndCooldownPowerup(float CooldownTime)
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &AActPowerupActor::ShowPowerup, CooldownTime);
}

void AActPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	//set visibility in root and all children
	RootComponent->SetVisibility(bNewIsActive, true);
}

void AActPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//implemented in derived classes
}



