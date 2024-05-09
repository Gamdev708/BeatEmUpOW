// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseSideFighterCharacter.h"
#include "Components/InputComponent.h"
#include <EnhancedInputComponent.h>
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ActionComponent.h"
#include "../../../Plugins/ComboSystemPlugin/Source/ComboSystemPlugin/Public/AttackStateMachine.h"

// Sets default values
ABaseSideFighterCharacter::ABaseSideFighterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 50.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	bIsDefending = false;


	AttackStateMachineComponent = CreateDefaultSubobject<UAttackStateMachine>("Attack StateMachine Component");
	ActionComp = CreateDefaultSubobject<UActionComponent>("Action Component");
		 
}

// Called when the game starts or when spawned
void ABaseSideFighterCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void ABaseSideFighterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseSideFighterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABaseSideFighterCharacter::StopJump);

		EnhancedInputComponent->BindAction(DuckAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::DoCrouch);
		EnhancedInputComponent->BindAction(DuckAction, ETriggerEvent::Completed, this, &ABaseSideFighterCharacter::DoUncrouch);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::Move);

		//Attacking
		EnhancedInputComponent->BindAction(LightAttackPunchAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::PunchLight);
		EnhancedInputComponent->BindAction(LightAttackKickAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::KickLight);

	}

}

void ABaseSideFighterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		if (GetCharacterMovement()->IsCrouching()) { return; }

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,0, Rotation.Yaw);

		// get forward vector
		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = -FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// add movement 
		/*AddMovementInput(ForwardDirection, MovementVector.Y);*/
		//GetCharacterMovement()->MaxWalkSpeed = 100.f;

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseSideFighterCharacter::StartJump()
{
	this->Jump();
}

void ABaseSideFighterCharacter::StopJump()
{
	this->StopJumping();
}

void ABaseSideFighterCharacter::DoCrouch()
{
	this->Crouch(false);
}
void ABaseSideFighterCharacter::DoUncrouch()
{
	this->UnCrouch(false);
}

void ABaseSideFighterCharacter::PunchLight()
{
}

void ABaseSideFighterCharacter::PunchMedium()
{
}

void ABaseSideFighterCharacter::PunchHeavy()
{
}

void ABaseSideFighterCharacter::PunchHeavyHold()
{
}

void ABaseSideFighterCharacter::KickLight()
{
}

void ABaseSideFighterCharacter::KickMedium()
{
}

void ABaseSideFighterCharacter::KickHeavy()
{
}

void ABaseSideFighterCharacter::KickHeavyHold()
{
}

