// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseSideFighterCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include <EnhancedInputComponent.h>
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ActionComponent.h"
#include "Components/HealthComponent.h"
#include "../../../Plugins/ComboSystemPlugin/Source/ComboSystemPlugin/Public/AttackStateMachine.h"
#include "../DebugHeader.h"

// Sets default values
ABaseSideFighterCharacter::ABaseSideFighterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GenerateHitboxesToSockets();
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 50.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	bIsDefending = false;
	bIsMovementHalted = false;
	CharacterModelTransform = FTransform();
	CharacterModelScale = FVector(0.0f, 0.0f, 0.0f);


	AttackStateMachineComponent = CreateDefaultSubobject<UAttackStateMachine>("Attack StateMachine Component");
	ActionComp = CreateDefaultSubobject<UActionComponent>("Action Component");
	HealthComp = CreateDefaultSubobject<UHealthComponent>("Health Component");
		 
}

void ABaseSideFighterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//DefendCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::CharacterCapsuleOverlapBegin);

	//DefendCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//DefendCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//DefendCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//DefendCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

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

// Called when the game starts or when spawned
void ABaseSideFighterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseSideFighterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OtherFighter)
	{
		// Get the direction to the opponent
		FVector DirectionToOpponent = OtherFighter->GetActorLocation() - GetActorLocation();
		DirectionToOpponent.Z = 0; // Ignore height difference

		// Get the forward vector of our character
		FVector ForwardVector = GetActorForwardVector();
		ForwardVector.Z = 0; // Ignore height difference

		// Calculate the angle between the two vectors
		float AngleDifference = FMath::Acos(FVector::DotProduct(ForwardVector, DirectionToOpponent.GetSafeNormal()));
		AngleDifference = FMath::RadiansToDegrees(AngleDifference);

		// If the angle is greater than 90 degrees, we need to turn
		if (AngleDifference > 90.0f)
		{
			// Calculate the new rotation
			FRotator NewRotation = DirectionToOpponent.Rotation();
			NewRotation.Pitch = 0;
			NewRotation.Roll = 0;

			// Set the new rotation
			SetActorRotation(NewRotation);
			//FRotator CurrentRotation = GetActorRotation();
			//FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, NewRotation, DeltaTime, 1.0f);
			//SetActorRotation(InterpolatedRotation);
		}
	}
}


void ABaseSideFighterCharacter::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) { return; }

	//Use a Interface instead
	if (TObjectPtr<ABaseSideFighterCharacter> Fighter = Cast<ABaseSideFighterCharacter>(OtherActor))
	{
		if (Fighter->GetIsDefending()) { return; }

		UHealthComponent* FigherHealth = Fighter->HealthComp;
		if (FigherHealth->ApplyHealthChange(this, GetAttackStateMachineComponent()->GetCurrentStateDamage()))
		{
			
		}
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

void ABaseSideFighterCharacter::CharacterCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsDefending) { return; }
	
	// You Probably want to add When character is Downed prevent this
	PlayAnimMontage(DefendMontage, DefendMontageSpeed);
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

