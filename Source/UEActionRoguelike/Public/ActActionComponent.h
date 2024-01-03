// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActAction.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ActActionComponent.generated.h"

class UActAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEACTIONROGUELIKE_API UActActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActActionComponent();
	
	//it needs include instead of forward declaration because it is a struct, not a pointer, needs to know the size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UActAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool RemoveAction(UActAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool RemoveActionByName(FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UActAction* GetActionByName(FName ActionName);
	
protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);
	
	// Granted abilities at game start
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UActAction>> DefaultActions;
	
	UPROPERTY()
	TArray<UActAction*> Actions;

	virtual void BeginPlay() override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
