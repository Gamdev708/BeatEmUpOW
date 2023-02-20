// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatEmUpOW/Characters/EnemyCharacter.h"
#include "MainCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerController.h"
#include "TimerManager.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Sphere"));
	CombatSphere->SetupAttachment(GetRootComponent());

	LeftHandCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Combat Collision"));
	LeftHandCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LeftHandHitBoxSocket"));
	
	RightHandCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Combat Collision"));
	RightHandCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightHandHitBoxSocket"));
	
	LeftLegCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Leg Combat Collision"));
	LeftLegCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LeftLegHitBoxSocket"));
	
	RightLegCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Leg Combat Collision"));
	RightLegCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightLegHitBoxSocket"));

	//CombatCollision->SetupAttachment(GetRootComponent());
	//HitParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("Hit Particle"));
	////HitParticle->SetupAttachment(GetRootComponent());

	AgroSphere->InitSphereRadius(600.f);
	CombatSphere->InitSphereRadius(150.f);

	bOverlappingCombatSphere = false;
	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 10.f;

	AttackminTime = 0.5f;
	AttackmaxTime = 3.5f;

	EEnemyMovementStatus::EMS_Idle;
	DeathDelay = 3.f;


	bHasValidTarget = false;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();


	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::CombatSphereOnOverlapEnd);

	LeftHandCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::CombatOverlapBegin);
	LeftHandCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::CombatOverlapEnd);

	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftHandCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftHandCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftHandCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightHandCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::CombatOverlapBegin);
	RightHandCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::CombatOverlapEnd);

	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightHandCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightHandCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	LeftLegCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::CombatOverlapBegin);
	LeftLegCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::CombatOverlapEnd);

	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftLegCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftLegCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	RightLegCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::CombatOverlapBegin);
	RightLegCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::CombatOverlapEnd);

	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightLegCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightLegCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AEnemyCharacter::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UCapsuleComponent* Playercap = Cast<UCapsuleComponent>(OtherComp);
	if (Playercap == nullptr) { return; }

	if (OtherActor && Alive())
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		{
			if (Main)
			{
				MoveToTarget(Main);
			}
		}

	}
}

void AEnemyCharacter::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		{
			if (Main)
			{
				bHasValidTarget = false;
				if (Main->CombatTarget == this)
				{
					Main->SetCombatTarget(nullptr);
				}
				Main->SetHasCombatTarget(false);
				Main->UpdateCombatTarget();
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
				if (AIController)
				{
					AIController->StopMovement();
				}
			}
		}

	}
}

void AEnemyCharacter::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UCapsuleComponent* Playercap = Cast<UCapsuleComponent>(OtherComp);
	if (Playercap == nullptr) { return; }

	if (OtherActor && Alive())
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			bHasValidTarget = true;
			Main->SetCombatTarget(this);
			Main->SetHasCombatTarget(true);

			Main->UpdateCombatTarget();
			
			CombatTarget = Main;
			bOverlappingCombatSphere = true;
			Attack();
		}
	}
}

void AEnemyCharacter::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	
	if (OtherActor && OtherComp)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		{
			if (Main)
			{

				bOverlappingCombatSphere = false;
				MoveToTarget(Main);
				CombatTarget = nullptr;

				if (Main->CombatTarget == this)
				{
					Main->SetCombatTarget(nullptr);
					Main->bHasCombatTarget = false;
					Main->UpdateCombatTarget();
				}
				if (Main->MainPlayerController)
				{
					USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
					//if (MainMesh) { Main->MainPlayerController->RemoveEnemeyHealthBar(); }
				}

				GetWorldTimerManager().ClearTimer(AttackTimer);
			}
		}

	}

}

void AEnemyCharacter::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			if (Main->HitParticle)
			{
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName(TEXT("TipSocket"));
				if (TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticle, SocketLocation, FRotator(0), false);
				}

			}
			if (Main->HitSound)
			{
				//UGameplayStatics::PlaySound2D(this, Main->HitSound);
			}
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}

		}
	}
}

void AEnemyCharacter::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemyCharacter::MoveToTarget(AMainCharacter* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToAttack);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);

		/** Testing for seeing navpoints of AI
		auto PathPoints=NavPath->GetPathPoints();
		for (auto Point :PathPoints)
		{
			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 1.5f);
		}
		*/
	}
}

void AEnemyCharacter::ActivateCollision()
{
	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//if (SwingSound)
	//{
	//	UGameplayStatics::PlaySound2D(this, SwingSound);
	//}
}

void AEnemyCharacter::DeactivateCollision()
{
	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacter::Attack()
{
	if (Alive() && bHasValidTarget)
	{
		
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if (!bAttacking)
		{
			
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				int32 Section = FMath::RandRange(0, 1);
				switch (Section)
				{
				case 0:AnimInstance->Montage_Play(CombatMontage, 2.24f);
					AnimInstance->Montage_JumpToSection(TEXT("Attack_1"), CombatMontage);
					break;
				case 1:AnimInstance->Montage_Play(CombatMontage, 1.8f);
					AnimInstance->Montage_JumpToSection(TEXT("Attack_2"), CombatMontage);
					break;
				case 2:AnimInstance->Montage_Play(CombatMontage, 1.8f);
					AnimInstance->Montage_JumpToSection(TEXT("Attack_3"), CombatMontage);
					break;
				default:
					break;
				}

			}
		}

	}
}

void AEnemyCharacter::AttackEnd()
{
	
	bAttacking = false;
	if (bOverlappingCombatSphere)
	{
		
		float AttackTime = FMath::FRandRange(AttackminTime, AttackmaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyCharacter::Attack, AttackTime);
		Attack();
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Health:%d"),Health);
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die(DamageCauser);
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void AEnemyCharacter::Die(AActor* Causer)
{


	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(TEXT("Death"));
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Death);
	LeftHandCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	AMainCharacter* Mian = Cast<AMainCharacter>(Causer);
	if (Mian)
	{
		Mian->UpdateCombatTarget();
	}
}

bool AEnemyCharacter::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Death;
}

void AEnemyCharacter::Dissappear()
{
	Destroy();
}

void AEnemyCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemyCharacter::Dissappear, DeathDelay);
}

