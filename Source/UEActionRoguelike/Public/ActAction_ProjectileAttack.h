// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActAction.h"
#include "ActAction_ProjectileAttack.generated.h"

class AActCharacter;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class UEACTIONROGUELIKE_API UActAction_ProjectileAttack : public UActAction
{
	GENERATED_BODY()


public:
	
	UActAction_ProjectileAttack();
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;// = TEXT("Muzzle_01");

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UParticleSystem> CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ShootingShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ShootingShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ShootingShakeOuterRadius;
	
	UFUNCTION()
	void AttackDelay_Elapsed(AActCharacter* InstigatorCharacter);
	
public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

};
