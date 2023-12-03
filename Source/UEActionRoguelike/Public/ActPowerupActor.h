// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ActPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class UEACTIONROGUELIKE_API AActPowerupActor : public AActor, public IActGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActPowerupActor();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	FTimerHandle TimerHandle_RespawnTimer;

	void ShowPowerup();

	void HideAndCooldownPowerup(float CooldownTime);

	void SetPowerupState(bool bNewIsActive);

public:	
	
	void Interact_Implementation(APawn* InstigatorPawn) override;
	
};
