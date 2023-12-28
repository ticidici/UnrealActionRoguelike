// Fill out your copyright notice in the Description page of Project Settings.


#include "ActActionEffect.h"

#include "ActActionComponent.h"

UActActionEffect::UActActionEffect()
{
	bAutoStart = true;
}

void UActActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if(Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UActActionEffect::StopAction_Implementation(AActor* Instigator)
{
	//if the period is about to be triggered but the duration timer comes first we still trigger the effect before stopping
	//Run before Super
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UActActionComponent* Comp = GetOwningComponent();
	if(Comp)
	{
		Comp->RemoveAction(this);
	}
}

void UActActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	//it can be empty and implemented in blueprint
}
