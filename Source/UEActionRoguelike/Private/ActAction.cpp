// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction.h"

#include "ActActionComponent.h"
#include "Logging/StructuredLog.h"

void UActAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOGFMT(LogTemp, Log, "Running: {Name}", GetNameSafe(this));

	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
}

void UActAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOGFMT(LogTemp, Log, "Stopped: {Name}", GetNameSafe(this));

	UActActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
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
