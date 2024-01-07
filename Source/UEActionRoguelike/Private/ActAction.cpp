// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction.h"

#include "ActActionComponent.h"
#include "Logging/StructuredLog.h"
#include "UEActionRoguelike/UEActionRoguelike.h"


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

	ensureAlways(bIsRunnning);
	
	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
	
	bIsRunnning = false;
}

UWorld* UActAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}


UActActionComponent* UActAction::GetOwningComponent() const
{
	return Cast<UActActionComponent>(GetOuter());
}

bool UActAction::IsRunning() const
{
	return bIsRunnning;
}

