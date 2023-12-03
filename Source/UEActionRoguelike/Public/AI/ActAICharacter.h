// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class UEACTIONROGUELIKE_API AActAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AActAICharacter();

protected:

	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UActAttributeComponent* AttributeComp;


	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashTimeParamName = TEXT("HitFlashTime");
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashSpeedParamName = TEXT("HitFlashSpeed");
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashColorParamName = TEXT("HitFlashColor");
	
	UPROPERTY(EditAnywhere, Category = "Receive Hit")
	float HitFlashSpeed;
	UPROPERTY(EditAnywhere, Category = "Receive Hit")
	FColor HitFlashColor;

	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta);
};
