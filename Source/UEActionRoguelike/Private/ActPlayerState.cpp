// Fill out your copyright notice in the Description page of Project Settings.


#include "ActPlayerState.h"

#include "Logging/StructuredLog.h"

AActPlayerState::AActPlayerState()
{
	Credits = 0;
}

void AActPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetCredits(0);//to notify whoever is listening to initialize the value
}

AActPlayerState* AActPlayerState::GetActPlayerState(APlayerController* FromPlayerController)
{
	if(FromPlayerController)
	{
		return FromPlayerController->GetPlayerState<AActPlayerState>();
	}

	return nullptr;
}

void AActPlayerState::SetCredits(int NewCredits)
{
	Credits = NewCredits;
	if(Credits < 0)
	{
		Credits = 0;
	}

	UE_LOGFMT(LogTemp, Log, "Set Credits directly. New number of credits {Number}", Credits);
	OnCreditsChanged.Broadcast(Credits, 0);
}

void AActPlayerState::VaryCredits(int CreditsVariation)
{
	Credits += CreditsVariation;
	if(Credits <= 0)
	{
		UE_LOGFMT(LogTemp, Log, "Credits less than 0, setting it to 0. actual number: {Number}", Credits);
		Credits = 0;
	}
	UE_LOGFMT(LogTemp, Log, "New number of credits {Number}", Credits);
	OnCreditsChanged.Broadcast(Credits, CreditsVariation);
}

int AActPlayerState::GetCredits()
{
	return Credits;
}
