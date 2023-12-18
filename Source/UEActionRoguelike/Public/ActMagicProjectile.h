// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActProjectileBase.h"
#include "GameFramework/Actor.h"
#include "ActMagicProjectile.generated.h"



UCLASS()
class UEACTIONROGUELIKE_API AActMagicProjectile : public AActProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AActMagicProjectile();

protected:

	UPROPERTY(EditAnywhere)
	float DamageAmount;//Positive
	UPROPERTY(EditAnywhere)
	float Knockback;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
};
