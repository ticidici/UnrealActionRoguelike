// Fill out your copyright notice in the Description page of Project Settings.


#include "ActAction_ProjectileAttack.h"

#include "ActCharacter.h"
#include "Kismet/GameplayStatics.h"

UActAction_ProjectileAttack::UActAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;

	ShootingShakeInnerRadius = 250.0f;
	ShootingShakeOuterRadius = 2500.0f;
}

void UActAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	AActCharacter* Character = Cast<AActCharacter>(Instigator);
	if(Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector:: ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FRotator ActorRotation = Character->GetActorRotation();
		FRotator ControllerRotation = Character->GetControlRotation();
		ActorRotation.Yaw = ControllerRotation.Yaw;
		Character->SetActorRotation(ActorRotation);
		
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

void UActAction_ProjectileAttack::AttackDelay_Elapsed(AActCharacter* InstigatorCharacter)
{
	if(ensureAlways(ProjectileClass))
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(),ShootingShake, InstigatorCharacter->GetActorLocation(), ShootingShakeInnerRadius, ShootingShakeOuterRadius);
		
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
		
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		//start cast a bit front of the camera
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (InstigatorCharacter->GetControlRotation().Vector() * 20);

		//endpoint far into the look-at distance
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);//assumes crosshair in center
		
		FHitResult Hit;
		if(GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd = Hit.ImpactPoint;
		}

		//FRotator ProjRotation = (TraceEnd - HandLocation).Rotation();
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
			
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	
	StopAction(InstigatorCharacter);
}


