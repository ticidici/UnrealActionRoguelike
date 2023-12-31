// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActInteractionComponent.generated.h"


class UActWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEACTIONROGUELIKE_API UActInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActInteractionComponent();

	void PrimaryInteract();
	
protected:

	// Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgement was received.
	//		Same idea as in TCP. Will stop processing subsequent calls until this is done correctly.
	// Unreliable - Not guaranteed, packet can get lost and won't retry.
	//		Same idea as in UDP. We would want this, for example, when updating something that gets called very often,
	//		like the position, which probably will change anyway in the next call
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	void FindBestInteractable();
	
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UActWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UActWorldUserWidget> DefaultWidgetInstance;
	
public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
