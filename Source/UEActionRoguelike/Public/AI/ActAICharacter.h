// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "ActAICharacter.generated.h"

class UPawnSensingComponent;
class UUserWidget;
class UActWorldUserWidget;

UCLASS()
class UEACTIONROGUELIKE_API AActAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AActAICharacter();

protected:

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UActAttributeComponent> AttributeComp;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	TObjectPtr<UActWorldUserWidget> ActiveHealthBar;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashTimeParamName;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashSpeedParamName;
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HitFlashColorParamName;
	
	UPROPERTY(EditAnywhere, Category = "Receive Hit")
	float HitFlashSpeed;
	UPROPERTY(EditAnywhere, Category = "Receive Hit")
	FColor HitFlashColor;


	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta);
};
