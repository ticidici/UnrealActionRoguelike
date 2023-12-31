// Fill out your copyright notice in the Description page of Project Settings.


#include "ActInteractionComponent.h"

#include "ActGameplayInterface.h"
#include "ActWorldUserWidget.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("act.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);


UActInteractionComponent::UActInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 50.f;
	TraceDistance = 500.f;
	CollisionChannel = ECC_WorldDynamic;
}


void UActInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UActInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if(MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UActInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	
	for(FHitResult	Hit: Hits)
	{
		if(bDebugDraw) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
		
		AActor* HitActor = Hit.GetActor();
		if(HitActor)
		{
			if(HitActor->Implements<UActGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}
	
	if(FocusedActor)
	{
		if(DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UActWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if(!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if(DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if(bDebugDraw) DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 0.0f, 2.0f);
}

void UActInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UActInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if(InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No focus actor to interact.");
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	IActGameplayInterface::Execute_Interact(InFocus, MyPawn);
}


