// Fill out your copyright notice in the Description page of Project Settings.


#include "ActCharacter.h"

#include "ActAttributeComponent.h"
#include "ActInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AActCharacter::AActCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");//string is only for visualization in editor
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractComp = CreateDefaultSubobject<UActInteractionComponent>("InteractComponent");

	AttributeComp = CreateDefaultSubobject<UActAttributeComponent>("AttributeComp");

    UCharacterMovementComponent* const L_CharacterMovement = GetCharacterMovement();
	L_CharacterMovement->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	JumpMaxCount = 1;
	JumpMaxHoldTime = 0.3f;
	L_CharacterMovement->JumpZVelocity = 800;
	L_CharacterMovement->GravityScale = 8;
	L_CharacterMovement->bApplyGravityWhileJumping = true;

	ImpactShakeInnerRadius = 250.0f;
	ImpactShakeOuterRadius = 2500.0f;
}

void AActCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &AActCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void AActCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AActCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(WantsToStopJump)
	{
		//is falling is actually like airborne, it can be moving upwards
		if(!GetCharacterMovement()->IsFalling())
		{
			WantsToStopJump = false;
		}
		else if(JumpForceTimeRemaining < (1 - 0.5f) * JumpMaxHoldTime)
		{
			WantsToStopJump = false;
			StopJumping();
		}
	}
}

// Called to bind functionality to input
void AActCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AActCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AActCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AActCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AActCharacter::SecondaryAttack);
	
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AActCharacter::DashProjectile);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AActCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AActCharacter::StopJump);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AActCharacter::PrimaryInteract);
}

void AActCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void AActCharacter::MoveForward(float Value /*has default in header*/)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void AActCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//X = Forward (Red)
	//Y = Right (Green)
	//Z = Up (Blue)
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void AActCharacter::PrepareAttack()
{
	PlayAnimMontage(AttackAnim);
	
	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	
	FRotator ActorRotation = GetActorRotation();
	FRotator ControllerRotation = GetControlRotation();
	ActorRotation.Yaw = ControllerRotation.Yaw;
	SetActorRotation(ActorRotation);
}

void AActCharacter::PrimaryAttack()
{
	PrepareAttack();
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AActCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void AActCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(MagicProjectileClass);
}

void AActCharacter::SecondaryAttack()
{
	PrepareAttack();

	//for now it's actually the same animation
	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &AActCharacter::SecondaryAttack_TimeElapsed, 0.2f);
}

void AActCharacter::SecondaryAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void AActCharacter::DashProjectile()
{
	PrepareAttack();

	//for now it's actually the same animation
	GetWorldTimerManager().SetTimer(TimerHandle_DashProjectile, this, &AActCharacter::DashProjectile_TimeElapsed, 0.2f);
}

void AActCharacter::DashProjectile_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void AActCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClass)
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(),ShootingShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
	
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	//Ignore Player
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	FVector TraceStart = CameraComp->GetComponentLocation();

	//endpoint far into the look-at distance
	FVector TraceEnd = CameraComp->GetComponentLocation() + GetControlRotation().Vector() * 5000;//assumes crosshair in center
	
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

void AActCharacter::Jump()
{
	Super::Jump();
}

void AActCharacter::StopJump()
{
	if(JumpKeyHoldTime < JumpMaxHoldTime)
	{
		if(JumpKeyHoldTime > 0.5f * JumpMaxHoldTime)
		{
			StopJumping();
		}
		else
		{
			WantsToStopJump = true;
		}
	}
}

void AActCharacter::PrimaryInteract()
{
	if(InteractComp)
	{
		InteractComp->PrimaryInteract();
	}
}

void AActCharacter::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta < 0.0f)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(),ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		GetMesh()->SetScalarParameterValueOnMaterials(HitFlashSpeedParamName, HitFlashSpeed);
		GetMesh()->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector(HitFlashColor.R, HitFlashColor.G, HitFlashColor.B));
		if(NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		}
	}
}

