// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "ActTargetDummy.generated.h"

class UActAttributeComponent;
class UStaticMeshComponent;//forward declaring it for consistency, even if it compiles without it

UCLASS()
class UEACTIONROGUELIKE_API AActTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UActAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth, float Delta, float ActualDelta, FGameplayTagContainer HealthVariationTags);

};
