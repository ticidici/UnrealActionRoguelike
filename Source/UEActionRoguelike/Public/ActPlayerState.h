// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ActPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditsChanged, int, NewCredits, int, Variation);

UCLASS()
class UEACTIONROGUELIKE_API AActPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AActPlayerState();
	
protected:
	
	virtual void PostInitializeComponents() override;
	
	int Credits;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Act Player State")
	static AActPlayerState* GetActPlayerState(APlayerController* FromPlayerController);
	
	void SetCredits(int NewCredits);
	void VaryCredits(int CreditsVariation);
	int GetCredits();

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
};
