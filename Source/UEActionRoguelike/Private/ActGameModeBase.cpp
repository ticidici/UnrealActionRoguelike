// Fill out your copyright notice in the Description page of Project Settings.


#include "ActGameModeBase.h"

#include "ActAttributeComponent.h"
#include "ActCharacter.h"
#include "ActPlayerState.h"
#include "EngineUtils.h"
#include "AI/ActAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
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

	//we cannot spawn the items right away because they depend on the nav mesh and it's not yet ready
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnInitialItems, this, &AActGameModeBase::SpawnInitialItemsTimerElapsed, 0.5f, false);
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
		UE_LOG(LogTemp,Log, TEXT("At maximum capacity, skipping bot spawn."));
		return;	
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AActGameModeBase::OnFindBotSpawnQueryCompleted);
	}
}

void AActGameModeBase::OnFindBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
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

void AActGameModeBase::SpawnInitialItemsTimerElapsed()
{
	SpawnItems();
}

void AActGameModeBase::SpawnItems()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnItemsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AActGameModeBase::OnFindItemsSpawnQueryCompleted);
	}
}

void AActGameModeBase::OnFindItemsSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn items EQS Query Failed!"))
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	int32 CoinSpawnCounter = 0;
	
	while(CoinSpawnCounter < NumberOfCoinsToSpawn && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		Locations.RemoveAt(RandomLocationIndex);
		
		GetWorld()->SpawnActor<AActor>(CoinClass, PickedLocation, FRotator::ZeroRotator);
		UE_LOG(LogTemp,Log, TEXT("Spawning coin at %f,%f,%f"), PickedLocation.X, PickedLocation.Y, PickedLocation.Z);
		
		CoinSpawnCounter++;
	}
	
	int32 PotionSpawnCounter = 0;

	while(PotionSpawnCounter < NumberOfPotionsToSpawn && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		Locations.RemoveAt(RandomLocationIndex);
		
		GetWorld()->SpawnActor<AActor>(PotionClass, PickedLocation, FRotator::ZeroRotator);
		UE_LOG(LogTemp,Log, TEXT("Spawning potion at %f,%f,%f"), PickedLocation.X, PickedLocation.Y, PickedLocation.Z);
		
		PotionSpawnCounter++;
	}
}

void AActGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AActCharacter* Player = Cast<AActCharacter>(VictimActor);
	AActAICharacter* Minion = Cast<AActAICharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 3.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);

		AActPlayerState* PlayerState = AActPlayerState::GetActPlayerState(Cast<APlayerController>(Player->GetController()));
		PlayerState->VaryCredits(-200);
	}
	else if(Minion)
	{
		UWorld* World = Minion->GetWorld();
		APlayerController* PlayerController = World->GetFirstLocalPlayerFromController()->GetPlayerController(World);
		AActPlayerState* PlayerState = AActPlayerState::GetActPlayerState(PlayerController);
		PlayerState->VaryCredits(+200);
	}
	
	UE_LOGFMT(LogTemp, Log, "OnActorKilled: Victim - {Victim}, Killer - {Killer}", GetNameSafe(VictimActor), GetNameSafe(Killer));
}
