// Fill out your copyright notice in the Description page of Project Settings.


#include "ActMagicProjectile.h"

#include "ActAttributeComponent.h"
#include "ActGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Logging/StructuredLog.h"

// Sets default values
AActMagicProjectile::AActMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AActMagicProjectile::OnActorOverlap);
	
	DamageAmount = 200;
	Knockback = 5000;
	
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
		// UActAttributeComponent* AttributeComp = UActAttributeComponent::GetAttributes(OtherActor);
		// if(AttributeComp)
		// {
		// 	int RandomDamage = FMath::Floor(FMath::RandRange(-100, 100));
		//
		// 	AttributeComp->ApplyHealthChange(GetInstigator(), -abs(DamageAmount + RandomDamage));
		// }
		// Explode();

		int RandomDamage = FMath::Floor(FMath::RandRange(-100, 100));

		//custom knockback impulse
		FVector CustomImpulse = SweepResult.TraceEnd - SweepResult.TraceStart;
		CustomImpulse.Normalize();
		CustomImpulse *= 2.f;//we give more contribution to real direction
		CustomImpulse += FVector_NetQuantizeNormal::UpVector;
		CustomImpulse.Normalize();
		CustomImpulse *= Knockback;
		
		if(!UActGameplayFunctionLibrary::ApplyDirectionalDamage(
			GetInstigator(), OtherActor, -abs(DamageAmount + RandomDamage), SweepResult, CustomImpulse, false
			))
		{
			UE_LOGFMT(LogTemp, Log,"Could not damage actor with magic projectile. Probably dead already.");
		}
		Explode();
	}
}

