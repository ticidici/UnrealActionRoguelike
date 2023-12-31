// Fill out your copyright notice in the Description page of Project Settings.


#include "ActItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


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

	//in runtime we would use SetReplicates(true), for pre-init the intended way is modifying the boolean
	bReplicates = true;
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
	//only the server gets here
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();//this is called automatically in the clients (ReplicatedUsing), but the server has to manually call it
}

void AActItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void AActItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AActItemChest, bLidOpened);
}

