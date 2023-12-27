// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ActGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRespawned, class AActCharacter*, NewCharacter);

UCLASS()
class UEACTIONROGUELIKE_API AActGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	//------ SPAWN MINIONS PERIODICALLY ------
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnvQuery> SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UCurveFloat> DifficultyCurve;
	
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnFindBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	//------ /SPAWN MINIONS PERIODICALLY ------


	//------ SPAWN INITIAL ITEMS ------
	
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<AActor> PotionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<AActor> CoinClass;

	UPROPERTY(EditAnywhere)
	int32 NumberOfPotionsToSpawn;
	UPROPERTY(EditAnywhere)
	int32 NumberOfCoinsToSpawn;
	
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TObjectPtr<UEnvQuery> SpawnItemsQuery;

	FTimerHandle TimerHandle_SpawnInitialItems;

	UFUNCTION()
	void SpawnInitialItemsTimerElapsed();

	UFUNCTION()
	void OnFindItemsSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void SpawnItems();
	
	//------ /SPAWN INITIAL ITEMS ------
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

public:

	AActGameModeBase();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerRespawned OnPlayerRespawned;
	
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
	virtual void StartPlay() override;
	
	UFUNCTION(Exec)
	void KillAll();
	
};
