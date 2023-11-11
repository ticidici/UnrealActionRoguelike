// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActProjectileBase.generated.h"

class USoundCue;
class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(Abstract)
class UEACTIONROGUELIKE_API AActProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActProjectileBase();

protected:

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* ImpactSFX;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* LoopSFX;

	UAudioComponent* LoopAudioComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	//BlueprintNativeEvent =  C++ base implementation, can be expanded in blueprints
	//BlueprintCallable to allow child classes to trigger explosions
	//not required for now but useful for expanding in blueprint later on
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;
};
