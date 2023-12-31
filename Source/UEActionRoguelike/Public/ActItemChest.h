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

	UPROPERTY(ReplicatedUsing = "OnRep_LidOpened", BlueprintReadOnly)//RepNotify
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Sets default values for this actor's properties
	AActItemChest();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//if a property is marked as Replicated, this method gets automatically declared in the header file without us having to do it
	//declared in ActItemChest.generated.h, we can still define it manually and won't cause errors
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
