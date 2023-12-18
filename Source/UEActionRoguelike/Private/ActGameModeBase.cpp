// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameModeBase.h"

#include "ActAttributeComponent.h"
#include "ActCharacter.h"
#include "EngineUtils.h"
#include "AI/ActAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Logging/StructuredLog.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("act.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

AActGameModeBase::AActGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AActGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots
	// Actual amount of bots and whether it is allowed to spawn determined by spawn logic later in the chain
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AActGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AActGameModeBase::KillAll()
{
	for(AActAICharacter* Bot : TActorRange<AActAICharacter>(GetWorld()))
	{
		UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(Bot);
		if(AttributeComp && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);//@fixme: pass in player? for kill credit
		}
	}
}

void AActGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOGFMT(LogTemp, Warning, "Bot spawning disabled via cvar 'CVarSpawnBots'.");
		return;
	}
	
	int32 NumberOfAliveBots = 0;

	//for(TActorIterator<AActAICharacter> It(GetWorld()); It; ++It) //alternative way
	for(AActAICharacter* Bot : TActorRange<AActAICharacter>(GetWorld()))
	{
		//	AActAICharacter* Bot = *It;
		UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(Bot);
		if(AttributeComp && AttributeComp->IsAlive())
		{
			NumberOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumberOfAliveBots);
	
	float MaxBotCount = 10.0f;
	
	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if(NumberOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp,Log, TEXT("At maximum capacity, skipping bot spawn."))
		return;	
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AActGameModeBase::OnQueryCompleted);
	}
}

void AActGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"))
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		UE_LOG(LogTemp,Log, TEXT("Spawning minion at %f,%f,%f"), Locations[0].X,Locations[0].Y,Locations[0].Z);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void AActGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);

		AActCharacter* NewCharacterPossessed = Cast<AActCharacter>(Controller->GetCharacter());
		OnPlayerRespawned.Broadcast(NewCharacterPossessed);
	}
}

void AActGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AActCharacter* Player = Cast<AActCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 3.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	UE_LOGFMT(LogTemp, Log, "OnActorKilled: Victim - {Victim}, Killer - {Killer}", GetNameSafe(VictimActor), GetNameSafe(Killer));
}
