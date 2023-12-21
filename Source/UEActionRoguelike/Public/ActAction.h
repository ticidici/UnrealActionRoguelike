// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ActAction.generated.h"

class UActActionComponent;
class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class UEACTIONROGUELIKE_API UActAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	UActActionComponent* GetOwningComponent() const;
	
	//Tags added to owning actor when activated, removed when action stops
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	//Action can only start if OwningActor has none of these tags applied
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	
public:

	//BlueprintNativeEvent has a c++ implementation but can be overriden in blueprints
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	// Action nickname to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;
};
