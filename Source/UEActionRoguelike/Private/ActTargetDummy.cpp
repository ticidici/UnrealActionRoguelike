// Fill out your copyright notice in the Description page of Project Settings.


#include "ActTargetDummy.h"

// Sets default values
AActTargetDummy::AActTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UActAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this, &AActTargetDummy::OnHealthChanged);
}


void AActTargetDummy::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta, float ActualDelta)
{
	//we use this for damage, not healing
	if(Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

