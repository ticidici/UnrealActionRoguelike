// Fill out your copyright notice in the Description page of Project Settings.


#include "ActItemChest.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AActItemChest::AActItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMseh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMseh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;
}

// Called when the game starts or when spawned
void AActItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
}
