// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPowerup_HealthPotion.h"

#include "ActAttributeComponent.h"

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
	
	UActAttributeComponent* AttributeComp = Cast<UActAttributeComponent>(InstigatorPawn->GetComponentByClass(UActAttributeComponent::StaticClass()));
	if(ensure(AttributeComp))
	{
		if(AttributeComp->isFullHealth())
		{
			//hide short time just to give some kind of feedback
			HideAndCooldownPowerup(1.0f);
		}
		else if(AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
		{
			HideAndCooldownPowerup(RespawnTime);
		}
	}
}
