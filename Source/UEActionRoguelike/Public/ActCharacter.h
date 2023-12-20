// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActCharacter.generated.h"

class UActInteractionComponent;
class USpringArmComponent;
class UCameraComponent;
class UActAttributeComponent;
class UActActionComponent;

UCLASS()
class UEACTIONROGUELIKE_API AActCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActCharacter();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;

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
	
	UPROPERTY(EditAnywhere, Category = "Receive Hit")
	float HitFlashSpeed;
	UPROPERTY(EditAnywhere, Category = "Receive Hit")
	FColor HitFlashColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UActAttributeComponent> AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UActActionComponent> ActionComp;

	
	bool WantsToStopJump = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UActInteractionComponent> InteractComp;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	void SecondaryAttack();
	void DashProjectile();
	void Jump();
	void StopJump();
	void PrimaryInteract();
	void SprintStart();
	void SprintStop();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta, float ActualDelta);

	virtual void PostInitializeComponents() override;
	
public:
	
	virtual FVector GetPawnViewLocation() const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 1000);
};
