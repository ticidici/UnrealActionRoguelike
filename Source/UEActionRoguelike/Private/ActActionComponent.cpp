// Fill out your copyright notice in the Description page of Project Settings.


#include "ActActionComponent.h"

#include "ActAction.h"

UActActionComponent::UActActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<UActAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}


void UActActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UActActionComponent::AddAction(TSubclassOf<UActAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	UActAction* NewAction = NewObject<UActAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UActActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UActAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}

bool UActActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UActAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}
	return false;
}
