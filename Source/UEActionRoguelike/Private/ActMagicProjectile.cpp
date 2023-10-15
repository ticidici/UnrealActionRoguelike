// Fill out your copyright notice in the Description page of Project Settings.


#include "ActMagicProjectile.h"

#include "ActAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AActMagicProjectile::AActMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AActMagicProjectile::OnActorOverlap);
	
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void AActMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		UActAttributeComponent* AttributeComp = Cast<UActAttributeComponent>(OtherActor->GetComponentByClass(UActAttributeComponent::StaticClass()));
		if(AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AActMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

