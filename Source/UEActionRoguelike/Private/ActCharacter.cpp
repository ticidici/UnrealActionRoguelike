// Fill out your copyright notice in the Description page of Project Settings.


#include "ActCharacter.h"

#include "ActAttributeComponent.h"
#include "ActInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AActCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AActCharacter::StopJump);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AActCharacter::PrimaryInteract);
}

void AActCharacter::MoveForward(float Value)
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

void AActCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AActCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void AActCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(MagicProjectileClass, SpawnTM, SpawnParams);
}

void AActCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);

	//for now it's actually the same animation
	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &AActCharacter::SecondaryAttack_TimeElapsed, 0.2f);
}

void AActCharacter::SecondaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(BlackHoleProjectileClass, SpawnTM, SpawnParams);
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
