// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction.h"

#include "ActActionComponent.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"
#include "UEActionRoguelike/UEActionRoguelike.h"


void UActAction::Initialize(UActActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool UActAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		//TODO this is already controlled by blocked tags, maybe remove and let an action cancel itself at some point in the animation
		UE_LOGFMT(LogTemp, Log, "Can't Start. Already Running: {Name}", GetNameSafe(this));
		return false;
	}
	
	UActActionComponent* Comp = GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

void UActAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOGFMT(LogTemp, Log, "Running: {Name}", GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
	
	bIsRunnning = true;
}

void UActAction::StopAction_Implementation(AActor* Instigator)
{
	//UE_LOGFMT(LogTemp, Log, "Stopped: {Name}", GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);

	//this ensure is not valid for online, as we are replicating the variable, and it changes in client before calling this method 
	//ensureAlways(bIsRunnning);
	
	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	
	bIsRunnning = false;
}

UWorld* UActAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

UActActionComponent* UActAction::GetOwningComponent() const
{
	return ActionComp;
}

void UActAction::OnRep_IsRunning()
{
	if(bIsRunnning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

bool UActAction::IsRunning() const
{
	return bIsRunnning;
}

void UActAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActAction, bIsRunnning);
	DOREPLIFETIME(UActAction, ActionComp);
}
