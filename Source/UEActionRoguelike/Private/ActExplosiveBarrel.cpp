// Fill out your copyright notice in the Description page of Project Settings.


#include "ActExplosiveBarrel.h"

#include "ActProjectileBase.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Logging/StructuredLog.h"

// Sets default values
AActExplosiveBarrel::AActExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetSimulatePhysics(true);
	// Enabling Simulate physics automatically changes the Profile to PhysicsActor in Blueprint, in C++ we need to change this manually.
	MeshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName); 
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComp->SetupAttachment(MeshComp);

	// Leaving this on applies small constant force via component 'tick' (Optional)
	RadialForceComp->SetAutoActivate(false);

	RadialForceComp->Radius = 750.0f;
	RadialForceComp->ImpulseStrength = 2500.0f; // Alternative: 200000.0 if bImpulseVelChange = false
	// Optional, ignores 'Mass' of other objects (if false, the impulse strength will be much higher to push most objects depending on Mass)
	RadialForceComp->bImpulseVelChange = true;

	// Optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AActExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//MeshComp->OnComponentHit.AddDynamic(this, &AActExplosiveBarrel::OnActorHit);
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AActExplosiveBarrel::OnActorOverlap);
}

void AActExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
	
	UE_LOGFMT(LogCore, Warning, "New normal log. Other actor: {OtherActorName}", GetNameSafe(OtherActor));

	FString CombinedString = FString::Printf(TEXT("Hit at Location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
	
	//UMeshComponent* OtherMesh = OtherActor->FindComponentByClass<UMeshComponent>();
	//if(OtherMesh != nullptr)
	//{
	//	FName CollisionName = OtherMesh->GetCollisionProfileName();
	//	if(CollisionName.IsValid() && CollisionName.Compare("Projectile"))
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Collision type: %s"), *CollisionName.ToString());
	//		//RadialForceComp->FireImpulse();
	//	}
	//}
}

void AActExplosiveBarrel::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<AActProjectileBase>(OtherActor))
	{
		RadialForceComp->FireImpulse();
	}
}


