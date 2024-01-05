// Fill out your copyright notice in the Description page of Project Settings.


#include "ActCharacter.h"

#include "ActActionComponent.h"
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
	ActionComp = CreateDefaultSubobject<UActActionComponent>("ActionComp");

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

FVector AActCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AActCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AActCharacter::SprintStop);
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

void AActCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AActCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "SecondaryAttack");
}

void AActCharacter::DashProjectile()
{
	ActionComp->StartActionByName(this, "DashProjectile");
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

void AActCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AActCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AActCharacter::OnHealthChanged(AActor* InstigatorActor, UActAttributeComponent* OwningComp, float NewHealth,
                                    float Delta, float ActualDelta)
{
	UKismetSystemLibrary::PrintString(this);

	if(Delta < 0.0f)
	{
		if(ActualDelta < 0.0f && !OwningComp->isFullRage())
		{
			constexpr float MaxHealthChange = 500.f;
			const float Ratio = FMath::Clamp(FMath::Abs(ActualDelta)/MaxHealthChange, 0, 1);
			constexpr float MaxRageChange = 300.f;
			const float RageChange = MaxRageChange * Ratio;
			
			OwningComp->ApplyRageChange(FMath::CeilToInt32(RageChange));
		}
		
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

