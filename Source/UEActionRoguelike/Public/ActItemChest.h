// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ActItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class UEACTIONROGUELIKE_API AActItemChest : public AActor, public IActGameplayInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere);
	float TargetPitch;
	
	void Interact_Implementation(APawn* InstigatorPawn);

protected:

UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Sets default values for this actor's properties
	AActItemChest();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
