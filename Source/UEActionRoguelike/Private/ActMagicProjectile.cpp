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
	
	DamageAmount = 200;
	
	MovementComp->InitialSpeed = 1000.0f;
}

void AActMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AActMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		UActAttributeComponent* AttributeComp = Cast<UActAttributeComponent>(OtherActor->GetComponentByClass(UActAttributeComponent::StaticClass()));
		if(AttributeComp)
		{
			int RandomDamage = FMath::Floor(FMath::RandRange(-100, 100));

			AttributeComp->ApplyHealthChange(-abs(DamageAmount + RandomDamage));
		}
		Explode();
	}
}

