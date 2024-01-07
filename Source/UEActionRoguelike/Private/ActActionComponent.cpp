// Fill out your copyright notice in the Description page of Project Settings.


#include "ActActionComponent.h"

#include "ActAction.h"
#include "Logging/StructuredLog.h"
#include "UEActionRoguelike/UEActionRoguelike.h"


UActActionComponent::UActActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UActActionComponent::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<UActAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


void UActActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, DebugMsg);

	//Draw All Actions
	for(UActAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s : IsRunning: %s : Outer: %s"),
			*GetNameSafe(GetOwner()),
			*Action->ActionName.ToString(),
			Action->IsRunning() ? TEXT("true") : TEXT("false"),
			*GetNameSafe(GetOuter()));

		LogOnScreen(this, ActionMsg, TextColor, 0.f);
	}
}


void UActActionComponent::AddAction(AActor* Instigator, TSubclassOf<UActAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	UActAction* NewAction = NewObject<UActAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		if (NewAction->ActionName == "None")
		{
			UE_LOGFMT(LogTemp, Error, "There's an Action with None as its ActionName.");
		}
		
		//we cancel the previous one if there is
		UActAction* OldAction = GetActionByName(NewAction->ActionName);
		if(OldAction)
		{
			OldAction->StopAction(GetOwner());
			RemoveAction(OldAction);
		}
		
		Actions.Add(NewAction);
		
		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

bool UActActionComponent::RemoveAction(UActAction* ActionToRemove)
{
	if(!ensure(ActionToRemove && ActionToRemove->IsRunning() == false))
	{
		return false;
	}
	
	Actions.Remove(ActionToRemove);
	return true;
}

bool UActActionComponent::RemoveActionByName(FName ActionName)
{
	for (UActAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			return RemoveAction(Action);
		}
	}
	return false;
}

bool UActActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UActAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			//we don't want the server to call itself in an infinite loop
			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}
			
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
			if(Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

UActAction* UActActionComponent::GetActionByName(FName ActionName)
{
	for (UActAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			return Action;
		}
	}
	return nullptr;
}

void UActActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

