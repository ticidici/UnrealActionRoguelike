// Fill out your copyright notice in the Description page of Project Settings.


#include "ActMagicProjectile.h"

#include "ActActionComponent.h"
#include "ActGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Logging/StructuredLog.h"
#include "ActActionEffect.h"

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
	APawn* OriginalInstigator = GetInstigator();
	if(OtherActor && OtherActor != OriginalInstigator)
	{
		UActActionComponent* ActionComp = Cast<UActActionComponent>(OtherActor->GetComponentByClass(UActActionComponent::StaticClass()));
		if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			//we try to return to the current position of the instigator instead of just turning it around
			FVector ReturnVelocity = -MovementComp->Velocity;
			if(OriginalInstigator)
			{
				float SpeedMagnitude = MovementComp->Velocity.Length();
				ReturnVelocity = OriginalInstigator->GetTransform().GetLocation() - OtherActor->GetTransform().GetLocation();
				ReturnVelocity.Normalize();
				ReturnVelocity *= SpeedMagnitude;
			}
			
			MovementComp->Velocity = ReturnVelocity;
			
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		int RandomDamage = FMath::Floor(FMath::RandRange(-100, 100));

		//custom knockback impulse
		FVector CustomImpulse = SweepResult.TraceEnd - SweepResult.TraceStart;
		CustomImpulse.Normalize();
		CustomImpulse *= 2.f;//we give more contribution to real direction
		CustomImpulse += FVector_NetQuantizeNormal::UpVector;
		CustomImpulse.Normalize();
		CustomImpulse *= Knockback;
		
		if(UActGameplayFunctionLibrary::ApplyDirectionalDamage(
			GetInstigator(), OtherActor, abs(DamageAmount + RandomDamage),
			SweepResult, CustomImpulse, FGameplayTagContainer::EmptyContainer, false
			))
		{
			if(ActionComp)
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
		}
		else
		{
			UE_LOGFMT(LogTemp, Log,"Could not damage actor with magic projectile. Probably dead already.");
		}
		Explode();
	}
}

