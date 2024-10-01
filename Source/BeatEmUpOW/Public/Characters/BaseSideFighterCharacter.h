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
class UHealthComponent;
class UAnimMontage;

UCLASS()
class BEATEMUPOW_API ABaseSideFighterCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true")) TObjectPtr<UAttackStateMachine > AttackStateMachineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") TObjectPtr<UActionComponent> ActionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") TObjectPtr<UHealthComponent> HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* LeftHandCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* RightHandCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* LeftLegCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* RightLegCombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") UBoxComponent* DefendCollision;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Defend") UAnimMontage* DefendMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Defend") float DefendMontageSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Model") FTransform CharacterModelTransform;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Model") FVector CharacterModelScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Model") bool isFlipped;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation") UAnimMontage* DeathMontage;



public:
	// Sets default values for this character's properties
	ABaseSideFighterCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") TObjectPtr<ABaseSideFighterCharacter> OtherFighter;


protected:
	virtual void PostInitializeComponents() override;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION() void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable) void ActivateCollision();
	UFUNCTION(BlueprintCallable) void DeactivateCollision();

	UFUNCTION() void CharacterCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bIsDefending;
	bool bIsMovementHalted;


	void GenerateHitboxesToSockets();

	FVector LastMovementInputStored;

public:
	FORCEINLINE TObjectPtr<UAttackStateMachine> GetAttackStateMachineComponent() const { return AttackStateMachineComponent; }
	FORCEINLINE TObjectPtr<UActionComponent> GetActionComponent() const { return ActionComp; }
	FORCEINLINE bool GetIsDefending() const { return bIsDefending; }

	FORCEINLINE bool GetIsMovementHalted() const { return bIsMovementHalted; }
	FORCEINLINE bool GetShouldDefend() const { return bIsDefending; }


	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsDefending(bool bShouldDefend) { bIsDefending = bShouldDefend; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetIsMovementHalted(bool bShouldMovementHalted) { bIsMovementHalted = bShouldMovementHalted; }
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetOtherFighter(ABaseSideFighterCharacter* Fighter) { this->OtherFighter = Fighter; }

	UFUNCTION(BlueprintCallable) FORCEINLINE FVector GetLastStoredMovementInput() { return LastMovementInputStored; }
	FORCEINLINE void SetLastStoredMovementInput(FVector MovementVector) { LastMovementInputStored = MovementVector; }

};
