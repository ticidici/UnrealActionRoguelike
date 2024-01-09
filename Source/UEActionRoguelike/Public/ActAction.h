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

	UPROPERTY(Replicated)
	TObjectPtr<UActActionComponent> ActionComp;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	UActActionComponent* GetOwningComponent() const;
	
	//Tags added to owning actor when activated, removed when action stops
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	//Action can only start if OwningActor has none of these tags applied
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_IsRunning")
	bool bIsRunnning;

	UFUNCTION()
	void OnRep_IsRunning();
	
public:

	void Initialize(UActActionComponent* NewActionComp);
	
	//Start immediately when added to an action component
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	//BlueprintNativeEvent has a c++ implementation but can be overriden in blueprints
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	// Action nickname to start/stop without a reference to the object
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

	FORCEINLINE virtual bool IsSupportedForNetworking() const override { return true; }
};
