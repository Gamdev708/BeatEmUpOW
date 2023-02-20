// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <SM_State.h>
#include "MainCharacter.generated.h"


class UFGInputDirectionalAtom;
class UFGButtonInputAtom;
class UFGMove;

UENUM(BlueprintType)
enum class ELightCombo :uint8
{
	ELC_None UMETA(DisplayName = "Normal"),
	ELC_Attack1  UMETA(DisplayName = "Light Attack 1"),
	ELC_Attack2 UMETA(DisplayName = "Light Attack 2"),
	ELC_Attack3 UMETA(DisplayName = "Light Attack 3"),
	ELC_Attack4 UMETA(DisplayName = "Light Attack 4")
};
UENUM(BlueprintType)
enum class EMediumCombo :uint8
{
	EMC_None UMETA(DisplayName = "Normal"),
	EMC_Attack1  UMETA(DisplayName = "Medium Attack 1"),
	EMC_Attack2 UMETA(DisplayName = "Medium Attack 2"),
	EMC_Attack3 UMETA(DisplayName = "Medium Attack 3"),
	EMC_Attack4 UMETA(DisplayName = "Medium Attack 4")
};
UENUM(BlueprintType)
enum class EHeavyCombo :uint8
{
	EHC_None UMETA(DisplayName = "Normal"),
	EHC_Attack1 UMETA(DisplayName = "Heavy Attack 1"),
	EHC_Attack2 UMETA(DisplayName = "Heavy Attack 2"),
	EHC_Attack3 UMETA(DisplayName = "Heavy Attack 3"),
	EHC_Attack4 UMETA(DisplayName = "Heavy Attack 4")
};

UCLASS()
class BEATEMUPOW_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")class AMainPlayerController* MainPlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Combat") float LightAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Combat") float MediumAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Combat") float HeavyAttackSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums") ELightCombo LightComboStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums") EMediumCombo MediumComboStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums") EHeavyCombo HeavyComboStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat") float Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")AController* WeaponInstigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") class UBoxComponent* LeftHandCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* RightHandCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* LeftLegCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* RightLegCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* TargetCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")bool bHasCombatTarget;
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat") class AEnemyCharacter* CombatTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat") class AEnemyCharacter* LockedOnTarget;
	FORCEINLINE void SetCombatTarget(AEnemyCharacter* Target) { CombatTarget = Target; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")class UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")class USoundCue* HitSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") TSubclassOf<AEnemyCharacter> EnemyFilter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") TArray<AEnemyCharacter*> LockedOnCandidates;

	/** Camera boom Positioning the Camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowedPrivateAcess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowedPrivateAcess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats") float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats") float MaxHealth;


	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TakeDown Combat") UBoxComponent HeavyAttackSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//FORCEINLINE float GetTimeInMove() const { return TimeInCurrentMove; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LightAttack();
	void LightAttackBTNPressed();
	void LightAttackBTNReleased();
	void HeavyAttack();
	void MediumAttack();
	void Grab();
	void Takedown();
	void PressJump(); //Remember Bind this Action in the Setup to capture the input

	void ATK_BTNUp();
	void UpdateCombatTarget();
	void ToggleLockOn();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims") UAnimMontage* LightComboMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims") UAnimMontage* MediumComboMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims") UAnimMontage* HeavyComboMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Anims") bool bAttacking;

	UFUNCTION(BlueprintCallable) void AttackEnd();
	UFUNCTION(BlueprintCallable) void ResetTransformRotation();
	UFUNCTION(BlueprintCallable) void DeathEnd();

	UFUNCTION(BlueprintCallable) void ActivateCollision();
	UFUNCTION(BlueprintCallable) void DeactivateCollision();
	UFUNCTION() void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION() void OnDetectedTargetsOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnDetectedTargetsOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:

	//void LightATKBTNPressed();
	//void LightATKBTNReleased();
	//void HeavyATKBTNPressed();
	//void HeavyATKBTNReleased();

	//FVector2D DirectionalInput;
	//uint32 ButtonDown;
	//uint32 ButtonDown_Old;
	//float TimeInCurrentMove;

	//UPROPERTY(EditAnywhere) UFGMove* CurrentMove;

	////Input atoms are removed when they pass this age threshold. All moves must be exceute in under this age
	//UPROPERTY(EditAnywhere) float InputExpriationTime;

	//UPROPERTY(EditAnywhere, Category = "Input Atoms") UFGInputDirectionalAtom* DirectionalBackForwardAtom;
	//UPROPERTY(EditAnywhere, Category = "Input Atoms") UFGInputDirectionalAtom* DirectionalForwardBackAtom;
	//UPROPERTY(EditAnywhere, Category = "Input Atoms") UFGInputDirectionalAtom* DirectionalBackwardAtom;
	////UPROPERTY(EditAnywhere, Category = "Input Atoms") UFGInputDirectionalAtom* DirectionalRightAtom;
	////UPROPERTY(EditAnywhere, Category = "Input Atoms") UFGInputDirectionalAtom* DirectionalLeftAtom;
	//UPROPERTY(EditAnywhere, Category = "Input Atoms") UFGInputDirectionalAtom* DirectionalForwardAtom;
	//UPROPERTY(EditAnywhere, Category = "Input Atoms") UFGInputDirectionalAtom* DirectionalNeutralAtom;

	//////Order in this array is the same as EFGButtonState
	//UPROPERTY(EditAnywhere, Category = "Input Atoms") TArray<UFGButtonInputAtom*> ButtonAtoms;
	//UFUNCTION(BlueprintImplementableEvent) void DoMove(UFGMove* NewMove);

private:

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void MakeCombo(ELightCombo LightCombo= ELightCombo::ELC_None, EMediumCombo MediumCombo = EMediumCombo::EMC_None, EHeavyCombo HeavyCombo = EHeavyCombo::EHC_None);
	void GenerateHitboxesToSockets();
	/// <summary>
	/// This is use to lock on to a single enemy
	/// </summary>
	void LockOnTarget();
	void Die();

	UPROPERTY(EditAnywhere) float RotationRate = 20;
	int Light;
	int Medium;
	int Heavy;
	float default_Combo_Timer;
	bool activateTimerToReset;
	float current_Combo_Timer;

	bool bATK_BTNDown;
	bool bIsLockedOnTarget;
	bool bIsDead;
	float TargetingHeightOffset;

	//~This array relates to InputStream.Input Stream must not be updated without this stream being updated
	/*UPROPERTY(VisibleInstanceOnly) TArray<float>InputTimeStamps;
	UPROPERTY(VisibleInstanceOnly) TArray<USM_InputAtom*> InputStream;*/
};	


//Add 
