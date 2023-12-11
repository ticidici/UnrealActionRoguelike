// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActCharacter.generated.h"

class UActInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UActAttributeComponent;

UCLASS()
class UEACTIONROGUELIKE_API AActCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackAnim;
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_SecondaryAttack;
	FTimerHandle TimerHandle_DashProjectile;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UParticleSystem> CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ShootingShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName = TEXT("TimeToHit");
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashSpeedParamName = TEXT("HitFlashSpeed");
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashColorParamName = TEXT("HitFlashColor");

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName = TEXT("Muzzle_01");
	
	UPROPERTY(EditAnywhere, Category = "Receive Hit")
	float HitFlashSpeed;
	UPROPERTY(EditAnywhere, Category = "Receive Hit")
	FColor HitFlashColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UActAttributeComponent> AttributeComp;

public:
	// Sets default values for this character's properties
	AActCharacter();
	
protected:

	bool WantsToStopJump = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UActInteractionComponent> InteractComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void SecondaryAttack();
	void SecondaryAttack_TimeElapsed();
	void DashProjectile();
	void DashProjectile_TimeElapsed();
	void Jump();
	void StopJump();
	void PrimaryInteract();

	void PrepareAttack();
	
	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 1000);
};
