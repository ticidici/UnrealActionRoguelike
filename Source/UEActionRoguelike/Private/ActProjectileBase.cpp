// Fill out your copyright notice in the Description page of Project Settings.


#include "ActProjectileBase.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AActProjectileBase::AActProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0;
	MovementComp->InitialSpeed = 8000;//just default, will change in most cases

	//bReplicates = true;
}

void AActProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &AActProjectileBase::OnActorHit);
}

void AActProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	if(LoopSFX) LoopAudioComponent = UGameplayStatics::SpawnSoundAttached(LoopSFX, RootComponent);
}

void AActProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

//"_Implementation" comes from it being marked as BlueprintNativeEvent
void AActProjectileBase::Explode_Implementation()
{
	//calling Destroy marks the actor as pending kill (deprecated, now use IsValid)
	//we use ensure to see if this happens at all, pointing to another potential problem
	if(ensure(IsValid(this)))
	{
		const FVector ProjectileLocation = GetActorLocation();
		if(ImpactVFX) UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, ProjectileLocation, GetActorRotation());
		if(ImpactSFX) UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, ProjectileLocation);
		if(LoopAudioComponent) LoopAudioComponent->Stop();
		Destroy();
	}
}
