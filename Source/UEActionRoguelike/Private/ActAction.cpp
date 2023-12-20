// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction.h"

#include "Logging/StructuredLog.h"

void UActAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOGFMT(LogTemp, Log, "Running: {Name}", GetNameSafe(this));
}

void UActAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOGFMT(LogTemp, Log, "Stopped: {Name}", GetNameSafe(this));
}
