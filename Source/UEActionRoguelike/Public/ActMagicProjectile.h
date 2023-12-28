// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActProjectileBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ActMagicProjectile.generated.h"


class UActActionEffect;

UCLASS()
class UEACTIONROGUELIKE_API AActMagicProjectile : public AActProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AActMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;//Positive
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Knockback;

	//Could be an array and be more generic
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UActActionEffect> BurningActionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
};
