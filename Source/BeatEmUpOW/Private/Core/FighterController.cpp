// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/FighterController.h"
#include "Characters/BaseSideFighterCharacter.h"
#include "Components/ActionComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "../../../Plugins/ComboSystemPlugin/Source/ComboSystemPlugin/Public/AttackStateMachine.h"
#include "../DebugHeader.h"

AFighterController::AFighterController()
{

}

void AFighterController::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AFighterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFighterController::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFighterController::StopJump);

		EnhancedInputComponent->BindAction(DuckAction, ETriggerEvent::Triggered, this, &AFighterController::DoCrouch);
		EnhancedInputComponent->BindAction(DuckAction, ETriggerEvent::Completed, this, &AFighterController::DoUncrouch);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AFighterController::DoDash);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFighterController::Move);

		//Attacking
		EnhancedInputComponent->BindAction(LightAttackPunchAction, ETriggerEvent::Started, this, &AFighterController::AttackLight);
		EnhancedInputComponent->BindAction(LightAttackPunchAction, ETriggerEvent::Started, this, &AFighterController::AttackLight);
		EnhancedInputComponent->BindAction(MediumAttackAction, ETriggerEvent::Started, this, &AFighterController::AttackMedium);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Canceled, this, &AFighterController::AttackHeavyPressed);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AFighterController::AttackHeavyTrigger);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Completed, this, &AFighterController::AttackHeavyReleased);


		//Cancelled - Press
		// triggered - When press time 
		// Compeleted - Decide with time and allow whether the Hold should be auto executed or damage reduced

	}
}
void AFighterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AFighterController::Move(const FInputActionValue& Value)
{

	if (PlayerCharacter->GetIsMovementHalted()) { return; }
	if (PlayerCharacter->GetCharacterMovement()->IsCrouching()) { return; }

	// Input is a Vector2D
	FVector MovementVector = Value.Get<FVector>();

	// Ensure OtherFighter is valid
	if (OtherFighter == nullptr) { return; }


	PlayerCharacter->SetLastStoredMovementInput(MovementVector);
	PlayerCharacter->AddMovementInput(MovementVector);

}

void AFighterController::StartJump()
{
	PlayerCharacter->GetActionComponent()->StartActionByName(PlayerCharacter, "JumpAction");
	//this->Jump();
}

void AFighterController::StopJump()
{
	PlayerCharacter->GetActionComponent()->StopActionByName(PlayerCharacter, "JumpAction");
	//this->StopJumping();
}

void AFighterController::DoCrouch()
{
	PlayerCharacter->GetActionComponent()->StartActionByName(PlayerCharacter, "CrouchAction");
	//this->Crouch(false);
}
void AFighterController::DoUncrouch()
{
	PlayerCharacter->GetActionComponent()->StopActionByName(PlayerCharacter, "CrouchAction");
	//this->UnCrouch(false);
}

void AFighterController::DoDash()
{
	PlayerCharacter->GetActionComponent()->StartActionByName(PlayerCharacter, "DashAction");
}

void AFighterController::AttackLight()
{
	AttackStateMachineComponent->SetInputState(1);
	AttackStateMachineComponent->SetInputButton(1);
}

void AFighterController::AttackMedium()
{
	AttackStateMachineComponent->SetInputState(1);
	AttackStateMachineComponent->SetInputButton(2);
}

void AFighterController::AttackHeavyPressed()
{
	AttackStateMachineComponent->SetInputState(1);
	AttackStateMachineComponent->SetInputButton(3);
}

void AFighterController::AttackHeavyTrigger()
{
	/*FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "KeyHoldTimerFunc", this);
	GetWorld()->GetTimerManager().SetTimer(KeyHoldPressTimerHandle, Delegate, KeyHoldPressTime, false);*/

}

void AFighterController::AttackHeavyReleased()
{
	AttackStateMachineComponent->SetInputState(2);
	AttackStateMachineComponent->SetInputButton(3);
}

void AFighterController::KeyHoldTimerFunc()
{

}
