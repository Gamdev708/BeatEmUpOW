// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseSideFighterCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
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

	GenerateHitboxesToSockets();
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
	
	LeftHandCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseSideFighterCharacter::CombatOverlapBegin);
	LeftHandCombatCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseSideFighterCharacter::CombatOverlapEnd);

	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftHandCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftHandCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightHandCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseSideFighterCharacter::CombatOverlapBegin);
	RightHandCombatCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseSideFighterCharacter::CombatOverlapEnd);

	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightHandCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	LeftLegCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseSideFighterCharacter::CombatOverlapBegin);
	LeftLegCombatCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseSideFighterCharacter::CombatOverlapEnd);


	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftLegCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftLegCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightLegCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseSideFighterCharacter::CombatOverlapBegin);
	RightLegCombatCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseSideFighterCharacter::CombatOverlapEnd);

	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightLegCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightLegCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
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
		EnhancedInputComponent->BindAction(LightAttackPunchAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::AttackLight);
		EnhancedInputComponent->BindAction(MediumAttackAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::AttackMedium);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::AttackHeavy);
		EnhancedInputComponent->BindAction(HeavyHoldAttackAction, ETriggerEvent::Triggered, this, &ABaseSideFighterCharacter::AttackHeavyHold);

	}

}

void ABaseSideFighterCharacter::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) { return; }
	if (TObjectPtr<ABaseSideFighterCharacter> Fighter = Cast<ABaseSideFighterCharacter>(OtherActor))
	{

	}
}

void ABaseSideFighterCharacter::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseSideFighterCharacter::ActivateCollision()
{
	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//Swing sound is initiaed in this stage and as well as any line renderes
}

void ABaseSideFighterCharacter::DeactivateCollision()
{
	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	//ActionComp->StartActionByName(this, "JumpAction");
	this->Jump();
}

void ABaseSideFighterCharacter::StopJump()
{
	//ActionComp->StopActionByName(this, "JumpAction");
	this->StopJumping();
}

void ABaseSideFighterCharacter::DoCrouch()
{
	//ActionComp->StartActionByName(this, "CrouchAction");
	this->Crouch(false);
}
void ABaseSideFighterCharacter::DoUncrouch()
{
	//ActionComp->StopActionByName(this, "CrouchAction");
	this->UnCrouch(false);
}

void ABaseSideFighterCharacter::AttackLight()
{
	//ActionComp->StopActionByName(this, "CrouchAction");
	AttackStateMachineComponent->SetInputState(1);
	AttackStateMachineComponent->SetInputButton(1);
}

void ABaseSideFighterCharacter::AttackMedium()
{
	//ActionComp->StopActionByName(this, "CrouchAction");
	AttackStateMachineComponent->SetInputState(1);
	AttackStateMachineComponent->SetInputButton(2);
}

void ABaseSideFighterCharacter::AttackHeavy()
{
	//ActionComp->StopActionByName(this, "CrouchAction");

	AttackStateMachineComponent->SetInputState(1);
	AttackStateMachineComponent->SetInputButton(3);
}

void ABaseSideFighterCharacter::AttackHeavyHold()
{
	//ActionComp->StopActionByName(this, "CrouchAction");
}

void ABaseSideFighterCharacter::GenerateHitboxesToSockets()
{
	LeftHandCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Combat Collision"));
	RightHandCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Combat Collision"));
	LeftLegCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Leg Combat Collision"));
	RightLegCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Leg Combat Collision"));

	LeftHandCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftHandHitBoxSocket"));
	RightHandCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightHandHitBoxSocket"));
	LeftLegCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftLegHitBoxSocket"));
	RightLegCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightLegHitBoxSocket"));

}

