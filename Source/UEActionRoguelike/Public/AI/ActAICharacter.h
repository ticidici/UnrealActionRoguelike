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

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
