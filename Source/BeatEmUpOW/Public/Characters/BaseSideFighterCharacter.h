// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "BaseSideFighterCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UBoxComponent;
class UParticleSystem;
class USoundCue;
class UAttackStateMachine;
class UActionComponent;

UCLASS()
class BEATEMUPOW_API ABaseSideFighterCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DuckAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LightAttackPunchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MediumAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyHoldAttackAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true")) TObjectPtr<UAttackStateMachine> AttackStateMachineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") TObjectPtr<UActionComponent> ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* LeftHandCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* RightHandCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* LeftLegCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* RightLegCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* DefendCollision;


public:
	// Sets default values for this character's properties
	ABaseSideFighterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION() void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable) void ActivateCollision();
	UFUNCTION(BlueprintCallable) void DeactivateCollision();

private:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void DoCrouch();
	void DoUncrouch();
	void AttackLight();
	void AttackMedium();
	void AttackHeavy();
	void AttackHeavyHold();
	bool bIsDefending;


	void GenerateHitboxesToSockets();

public:
	FORCEINLINE TObjectPtr<UAttackStateMachine> GetAttackStateMachineComponent() const { return AttackStateMachineComponent; }
	FORCEINLINE TObjectPtr<UActionComponent> GetActionComponent() const { return ActionComp; }
};
