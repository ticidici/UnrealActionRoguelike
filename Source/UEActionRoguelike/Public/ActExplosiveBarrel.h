// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class UEACTIONROGUELIKE_API AActExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActExplosiveBarrel();

protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForceComp;

	// Must be marked with ufunction in order to 'bind' the event
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void PostInitializeComponents() override;

};
