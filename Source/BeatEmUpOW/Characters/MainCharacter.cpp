// Fill out your copyright notice in the Description page of Project Settings.

#include "BeatEmUpOW/Characters/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "BeatEmUpOW/FGAtoms.h"
#include <BeatEmUpOW/FGMove.h>
#include "EnemyCharacter.h"
#include "MainPlayerController.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//InputExpriationTime = 2.0f;



	GenerateHitboxesToSockets();

	TargetCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Lockon Detection Collision Box"));
	TargetCollision->SetupAttachment(GetMesh());


	//Create Camera Boom- Pulls towards the player if there's a collision
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; //Camera Follow Distance
	CameraBoom->bUsePawnControlRotation = true; //Rotate arm based on Controller
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match
	//Controller Orientation
	//FollowCamera->bUsePawnControlRotation = false;


	//Don't rotate when the controller rotates
	//Let camera be affect
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationRoll = false;

	//Configure Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true; //Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); //.... at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	LightAttackSpeed = 2.f;
	MediumAttackSpeed = 2.5f;
	HeavyAttackSpeed = 2.8f;

	LightComboStatus = ELightCombo::ELC_None;
	MediumComboStatus = EMediumCombo::EMC_None;
	HeavyComboStatus = EHeavyCombo::EHC_None;

	Light = 0;
	Medium = 0;
	Heavy = 0;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	activateTimerToReset = false;
	default_Combo_Timer = 1.f;
	current_Combo_Timer = default_Combo_Timer;

	bAttacking = false;
	bATK_BTNDown = false;


	bIsLockedOnTarget = false;
	TargetingHeightOffset = 20.f;
	CombatTarget = nullptr;
	LockedOnTarget = nullptr;

	MaxHealth = 100.f;
	Health = 100.f;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	LeftHandCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::CombatOverlapBegin);
	LeftHandCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::CombatOverlapEnd);

	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftHandCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftHandCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightHandCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::CombatOverlapBegin);
	RightHandCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::CombatOverlapEnd);

	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightHandCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	LeftLegCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::CombatOverlapBegin);
	LeftLegCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::CombatOverlapEnd);


	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftLegCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftLegCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightLegCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::CombatOverlapBegin);
	RightLegCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::CombatOverlapEnd);

	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightLegCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightLegCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	TargetCollision->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnDetectedTargetsOverlapBegin);
	TargetCollision->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::OnDetectedTargetsOverlapEnd);

}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (activateTimerToReset)
	{
		current_Combo_Timer -= DeltaTime;
		if (current_Combo_Timer <= 0.f)
		{
			activateTimerToReset = false;
			current_Combo_Timer = default_Combo_Timer;
			Light = 0;
			LightComboStatus = StaticCast<ELightCombo>(Light);
			Medium = 0;
			MediumComboStatus = StaticCast<EMediumCombo>(Medium);
			Heavy = 0;
			HeavyComboStatus = StaticCast<EHeavyCombo>(Heavy);
		}
	}


	if (bIsLockedOnTarget)
	{
		FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedOnTarget->GetActorLocation());
		lookAtRotation.Pitch -= TargetingHeightOffset;
		GetController()->SetControlRotation(lookAtRotation);
	}
//	// ============================================ THIS IS THE STATE MACHINE CODE USED BEATM SYS=====================
//	//Process Input
//
//	//Add one atom for stick information
//	const float DirectionalThreshold = 0.5f;
//	UFGInputDirectionalAtom* InputDirection = nullptr;
//	if (DirectionalInput.X< -DirectionalThreshold)
//	{
///*		if (DirectionalInput.Y<-DirectionalThreshold)
//		{
//			InputDirection = DirectionalBackForwardAtom;
//		}
//		else*/ if (DirectionalInput.Y < DirectionalThreshold)
//		{
//			InputDirection = DirectionalBackwardAtom;
//		}
//		//else
//		//{
//		//	InputDirection = DirectionalForwardAtom;
//		//}
//	}
//	else if(DirectionalInput.X<DirectionalThreshold)
//	{
///*		if (DirectionalInput.Y < -DirectionalThreshold)
//		{
//			InputDirection = DirectionalBackwardAtom;
//		}
//		else*/ if (DirectionalInput.Y < DirectionalThreshold)
//		{
//			InputDirection = DirectionalNeutralAtom;
//		}
//		/*else
//		{
//			InputDirection = DirectionalForwardAtom;
//		}*/
//	}
//	else
//	{
//		/*if (DirectionInput.Y < -DirectionThreshold)
//		{
//			InputDirection = DirectionDownForwardAtom;
//		}
//		else*/ if (DirectionalInput.Y < DirectionalThreshold)
//		{
//			InputDirection = DirectionalForwardAtom;
//		}
//		/*else
//		{
//			InputDirection = DirectionUpForwardAtom;
//		}*/
//	}
//	InputStream.Add(InputDirection);
////	
////
////	//Add one atom for eachbutton's State
//	for (int32 i = 0; i < (int32)EFGInputButtons::Count; ++i)
//	{
//
//		if (ButtonDown &(1<<i))
//		{
//			
//			if (ButtonDown_Old &(1<<i))
//			{
//				//UE_LOG(LogTemp, Warning, TEXT("Shifting to Held Down = %d"), 1 << i);
//				InputStream.Add(ButtonAtoms[(int32)EFGButtonState::HeldDown]);
//				
//			}
//			else
//			{
//				//UE_LOG(LogTemp, Warning, TEXT("Shifting pressed = %d"), 1 << i);
//				InputStream.Add(ButtonAtoms[(int32)EFGButtonState::JustPressed]);
//				
//			}
//		}
//		else
//		{
//			//UE_LOG(LogTemp, Warning, TEXT("Shifting UP= %d"), 1 << i);
//			InputStream.Add(ButtonAtoms[(int32)EFGButtonState::Up]);
//		}
//	}
//
//	//Always add an input time stamp to match the input sequence.
//	float CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);
//	InputTimeStamps.Add(CurrentTime);
//
//	//Cache old buttons so we can distinguish between held down and just pressed
//	ButtonDown_Old = ButtonDown;
//
//	//Prune old inputs. This would be better suited to a ring buffer than an array, but it's not much data
//	for (int32 i = 0; i < InputStream.Num(); i++)
//	{
//
//		if ((InputTimeStamps[i]+InputExpriationTime)>=CurrentTime)
//		{
//			//Remove everything before this then exit the loop
//			if (i > 0)
//			{
//				InputTimeStamps.RemoveAt(0, i, false);
//				InputStream.RemoveAt(0, i * ((int32)EFGInputButtons::Count + 1), false);
//			}
//			break;
//		}
//	}
//
//	FFGMoveLinkToFollow MoveLinkToFollow = CurrentMove->TryLinks(this, InputStream);
//	if (MoveLinkToFollow.SMR.CompeletionType==EStateMachineCompletionType::Accepted)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Switching to state %s"), *MoveLinkToFollow.Link->Move->MoveName.ToString());
//		if (MoveLinkToFollow.Link->bClearInput||MoveLinkToFollow.Link->Move->bClearInputOnEntry|| CurrentMove->bClearInputOnExit)
//		{
//			InputStream.Reset();
//			InputTimeStamps.Reset();
//		}
//		else if (MoveLinkToFollow.SMR.DataIndex)
//		{
//			//Consume the input we used to get to this move
//			check((MoveLinkToFollow.SMR.DataIndex % (1 + (int32)EFGInputButtons::Count)) == 0);
//			InputTimeStamps.RemoveAt(0, MoveLinkToFollow.SMR.DataIndex/3, false);
//			InputStream.RemoveAt(0, MoveLinkToFollow.SMR.DataIndex, false);
//		}
//
//		//Set and start the new move
//		CurrentMove = MoveLinkToFollow.Link->Move;
//		TimeInCurrentMove = 0.f;
//		DoMove(CurrentMove);
//	}
//	else
//	{
//		TimeInCurrentMove += DeltaTime; //Modulate by move animation length
//	}
//
//	//============================================ THIS IS THE END OF STATE MACHINE CODE USED BEATM SYS==================== =
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

#pragma region Locomotion

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AMainCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AMainCharacter::LookRightRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	/*PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AMainCharacter::LookRightRate);
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AMainCharacter::LookUpRate);*/

#pragma endregion

#pragma region Combat

	//PlayerInputComponent->BindAction(TEXT("LightAttack"), IE_Pressed, this, &AMainCharacter::LightAttackBTNPressed);
	//PlayerInputComponent->BindAction(TEXT("LightAttack"), IE_Released, this, &AMainCharacter::LightAttackBTNReleased);
	PlayerInputComponent->BindAction(TEXT("LightAttack"), IE_Pressed, this, &AMainCharacter::LightAttack);
	PlayerInputComponent->BindAction(TEXT("LightAttack"), IE_Released, this, &AMainCharacter::ATK_BTNUp);
	PlayerInputComponent->BindAction(TEXT("MediumAttack"), IE_Pressed, this, &AMainCharacter::MediumAttack);
	PlayerInputComponent->BindAction(TEXT("MediumAttack"), IE_Released, this, &AMainCharacter::ATK_BTNUp);
	PlayerInputComponent->BindAction(TEXT("HeavyAttack"), IE_Pressed, this, &AMainCharacter::HeavyAttack);
	PlayerInputComponent->BindAction(TEXT("HeavyAttack"), IE_Released, this, &AMainCharacter::ATK_BTNUp);
	PlayerInputComponent->BindAction(TEXT("LockOnTarget"), IE_Pressed, this, &AMainCharacter::ToggleLockOn);
	//PlayerInputComponent->BindAction(TEXT("Grab"), IE_Pressed, this, &AMainCharacter::Grab);
	//PlayerInputComponent->BindAction(TEXT("Takedown"), IE_Pressed, this, &AMainCharacter::Takedown);
	//Take Down is used for silent takedowns not for all

#pragma endregion


}

void AMainCharacter::LightAttack()
{
	//Make this funvtion
	//SetInterpToEnemy(true);
	if (bIsDead) { return; }
	bATK_BTNDown = true;

	if (!bAttacking)
	{
		bAttacking = true;
		if (LightComboStatus == ELightCombo::ELC_Attack4 || MediumComboStatus == EMediumCombo::EMC_Attack4 || HeavyComboStatus == EHeavyCombo::EHC_Attack4)
		{
			Light = 0;
			LightComboStatus = StaticCast<ELightCombo>(Light);
			return;
		}
		Light++;
		LightComboStatus = StaticCast<ELightCombo>(Light);


		UE_LOG(LogTemp, Warning, TEXT("Light Attack:%d"), Light);


		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && LightComboMontage) {
			switch (LightComboStatus)
			{
			case ELightCombo::ELC_None:
				break;
			case ELightCombo::ELC_Attack1:
				AnimInstance->Montage_Play(LightComboMontage, LightAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("LightAttack_1"), LightComboMontage);
				current_Combo_Timer = default_Combo_Timer;
				activateTimerToReset = true;
				break;
			case ELightCombo::ELC_Attack2:
				AnimInstance->Montage_Play(LightComboMontage, LightAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("LightAttack_2"), LightComboMontage);
				current_Combo_Timer = default_Combo_Timer;
				activateTimerToReset = true;
				break;
			case ELightCombo::ELC_Attack3:
				AnimInstance->Montage_Play(LightComboMontage, LightAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("LightAttack_3"), LightComboMontage);
				current_Combo_Timer = default_Combo_Timer;
				activateTimerToReset = true;
				break;
			case ELightCombo::ELC_Attack4:
				AnimInstance->Montage_Play(LightComboMontage, LightAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("LightAttack_4"), LightComboMontage);
				current_Combo_Timer = default_Combo_Timer;
				activateTimerToReset = true;
				break;
			default:
				break;
			}
		}
	}
}
void AMainCharacter::ResetTransformRotation() 
{
	FQuat Rotation(0, 0, 0, 0);
	SetActorRelativeRotation(Rotation);
	UE_LOG(LogTemp, Warning, TEXT("REST Transform"));
}
void AMainCharacter::LightAttackBTNPressed()
{
	//ButtonDown |= (1 << (int32)EFGInputButtons::LightAttack);
}

void AMainCharacter::LightAttackBTNReleased()
{
	//ButtonDown &= ~(1 << (int32)EFGInputButtons::LightAttack);
}

void AMainCharacter::MediumAttack()
{
	if (bIsDead) { return; }
	bATK_BTNDown = true;
	if (!bAttacking) {

		bAttacking = true;

		if (LightComboStatus == ELightCombo::ELC_Attack4 || MediumComboStatus == EMediumCombo::EMC_Attack4 || HeavyComboStatus == EHeavyCombo::EHC_Attack4)
		{
			Medium = 0;
			MediumComboStatus = StaticCast<EMediumCombo>(Medium);
			return;
		}
		Medium++;
		MediumComboStatus = StaticCast<EMediumCombo>(Medium);

		UE_LOG(LogTemp, Warning, TEXT("Medium Attack:%d"), Medium);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && MediumComboMontage) {
			switch (MediumComboStatus)
			{
			case EMediumCombo::EMC_None:
				break;
			case EMediumCombo::EMC_Attack1:
				AnimInstance->Montage_Play(MediumComboMontage, MediumAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("MediumAttack_1"), MediumComboMontage);
				current_Combo_Timer = default_Combo_Timer + 0.5f;
				activateTimerToReset = true;
				break;
			case EMediumCombo::EMC_Attack2:
				AnimInstance->Montage_Play(MediumComboMontage, MediumAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("MediumAttack_2"), MediumComboMontage);
				current_Combo_Timer = default_Combo_Timer + 0.5f;
				activateTimerToReset = true;
				break;
			case EMediumCombo::EMC_Attack3:
				AnimInstance->Montage_Play(MediumComboMontage, MediumAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("MediumAttack_3"), MediumComboMontage);
				current_Combo_Timer = default_Combo_Timer + 0.5f;
				activateTimerToReset = true;
				break;
			case EMediumCombo::EMC_Attack4:
				AnimInstance->Montage_Play(MediumComboMontage, MediumAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("MediumAttack_4"), MediumComboMontage);
				current_Combo_Timer = default_Combo_Timer + 0.5f;
				activateTimerToReset = true;
				break;
			default:
				break;
			}
		}

	}

}

void AMainCharacter::HeavyAttack()
{
	if (bIsDead) { return; }
	bATK_BTNDown = true;
	if (!bAttacking) {
		bAttacking = true;

		if (LightComboStatus == ELightCombo::ELC_Attack4 || MediumComboStatus == EMediumCombo::EMC_Attack4 || HeavyComboStatus == EHeavyCombo::EHC_Attack4)
		{
			Heavy = 0;
			HeavyComboStatus = StaticCast<EHeavyCombo>(Heavy);
			return;
		}
		Heavy++;
		HeavyComboStatus = StaticCast<EHeavyCombo>(Heavy);

		UE_LOG(LogTemp, Warning, TEXT("Heavy Attack:%d"), Heavy);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && MediumComboMontage) {
			switch (HeavyComboStatus)
			{
			case EHeavyCombo::EHC_None:
				break;
			case EHeavyCombo::EHC_Attack1:
				AnimInstance->Montage_Play(HeavyComboMontage, HeavyAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("HeavyAttack_1"), HeavyComboMontage);
				current_Combo_Timer = default_Combo_Timer + 0.6f;
				activateTimerToReset = true;
				break;
			case EHeavyCombo::EHC_Attack2:
				AnimInstance->Montage_Play(HeavyComboMontage, HeavyAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("HeavyAttack_2"), HeavyComboMontage);
				current_Combo_Timer = default_Combo_Timer + 0.6f;
				activateTimerToReset = true;
				break;
			case EHeavyCombo::EHC_Attack3:
				AnimInstance->Montage_Play(HeavyComboMontage, HeavyAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("HeavyAttack_3"), HeavyComboMontage);
				current_Combo_Timer = default_Combo_Timer + 0.6f;
				activateTimerToReset = true;
				break;
			case EHeavyCombo::EHC_Attack4:
				AnimInstance->Montage_Play(HeavyComboMontage, HeavyAttackSpeed);
				AnimInstance->Montage_JumpToSection(TEXT("HeavyAttack_4"), HeavyComboMontage);
				current_Combo_Timer = default_Combo_Timer + 0.6f;
				activateTimerToReset = true;
				break;
			default:
				break;
			}
		}

	}



}

void AMainCharacter::Grab()
{
}

void AMainCharacter::Takedown()
{
}

void AMainCharacter::PressJump()
{
	
}

void AMainCharacter::ATK_BTNUp()
{
	bATK_BTNDown = false;
}

void AMainCharacter::AttackEnd()
{
	bAttacking = false;
	//SetInterpToEnemy(false);
	/*if (bATK_BTNDown)
	{
		Attack();   //IF this is need for the Heavy or MID
	}*/
}



void AMainCharacter::MoveForward(float AxisValue)
{
	if (bIsDead) { return; }
	AddMovementInput(GetActorForwardVector() * AxisValue);
	//DirectionalInput.Y = AxisValue;
}

void AMainCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
	//DirectionalInput.X = AxisValue;
}

void AMainCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::MakeCombo(ELightCombo LightCombo, EMediumCombo MediumCombo, EHeavyCombo HeavyCombo)
{
	
}

void AMainCharacter::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	//ENemy Detecttion Hit Code170
	if (OtherActor)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
		if (Enemy)
		{
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, GetController(), this, DamageTypeClass);
			}
		}

		/*AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			if (Enemy->HitParticle)
			{
				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName(TEXT("WeaponSocket"));
				if (WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticle, SocketLocation, FRotator(0), false);
				}

			}
			if (Enemy->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
			}
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
			}
		}*/
	}
}
void AMainCharacter::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMainCharacter::OnDetectedTargetsOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);

	if (Enemy)
	{
		LockedOnCandidates.AddUnique(Enemy);
	}
	
}
void AMainCharacter::OnDetectedTargetsOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);

	if (Enemy)
	{
		LockedOnCandidates.Remove(Enemy);
	}
}


// TO DO: MUST USE seperate ACTIVATE DEACTIVE COllsions for the each part of the body 


void AMainCharacter::ActivateCollision() {
	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Swing sound is initiaed in this stage and as well as any line renderes
}
void AMainCharacter::DeactivateCollision(){
	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacter::GenerateHitboxesToSockets() {

	LeftHandCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Combat Collision"));
	RightHandCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Combat Collision"));
	LeftLegCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Leg Combat Collision"));
	RightLegCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Leg Combat Collision"));

	LeftHandCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftHandHitBoxSocket"));
	RightHandCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightHandHitBoxSocket"));
	LeftLegCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftLegHitBoxSocket"));
	RightLegCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightLegHitBoxSocket"));
}

void AMainCharacter::UpdateCombatTarget()
{
	//if (bIsLockedOnTarget) { return; }
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			//MainPlayerController->RemoveEnemeyHealthBar();
		}
		return;
	}

	AEnemyCharacter* ClosestEnemy = Cast<AEnemyCharacter>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}

		}
		if (MainPlayerController)
		{
			//MainPlayerController->DisplayEnemeyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
	

}


void AMainCharacter::LockOnTarget() {


	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);

	if (OverlappingActors.Num() != 0)
	{
		if (MainPlayerController)
		{
			//MainPlayerController->RemoveEnemeyHealthBar();
		}
		return;
	}
	AEnemyCharacter* ClosestEnemy = Cast<AEnemyCharacter>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}

		}
		if (MainPlayerController)
		{
			//MainPlayerController->DisplayEnemeyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}


void AMainCharacter::ToggleLockOn() {
	if (bIsLockedOnTarget)
	{
		bIsLockedOnTarget = false;
		LockedOnTarget = nullptr;
	}
	else
	{
		if (LockedOnCandidates.Num()>0)
		{
			LockedOnTarget = LockedOnCandidates[0];
			if (LockedOnTarget)
			{
				bIsLockedOnTarget = true;
			}
		}
	}
}


float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	UE_LOG(LogTemp, Warning, TEXT("Enemy Health:%d"), Health);
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void AMainCharacter::Die()
{
	if (bIsDead) { return; }
	DetachFromControllerPendingDestroy();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(LightComboMontage, 1.f);
		AnimInstance->Montage_JumpToSection(TEXT("Death"));
	}
	bIsDead = true;
}

void AMainCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}