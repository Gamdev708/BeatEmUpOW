// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/State.h"
#include "AttackBaseState.generated.h"


class UAttackStateMachine;


UENUM(BlueprintType)
enum class EButtonState : uint8
{
	None,
	Pressed,
	Hold,
	Tap,
	Invalid,
	// Add more transitions here as needed
};

UENUM(BlueprintType)
enum class EInputButtons : uint8
{
	None,
	LightAttack,
	MediumAttack,
	HeavyAttack,
	Invalid,
	// Add more transitions here as needed
};



/**
 * 
 */
UCLASS()
class COMBOSYSTEMPLUGIN_API UAttackBaseState : public UState
{
	GENERATED_BODY()

public:
	UAttackBaseState();
	FORCEINLINE void Initialize(UAttackStateMachine* NewAttackStateMachine) { AttackStateMachine = NewAttackStateMachine; }
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Enter", Category = "Base State Functions") void OnEnter();
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Update", Category = "Base State Functions") void OnUpdate(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Exit", Category = "Base State Functions") void OnExit();

	float GetDamage() { return Damage; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Combo Settings") UAttackStateMachine* AttackStateMachine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation") UAnimMontage* Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Settings") float Damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Settings") float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo Settings") TArray<EInputButtons> RequiredInputs;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo Settings") TArray<EButtonState> RequiredStates;


	bool HandleInput(EInputButtons EnteredInput, EButtonState EnteredState);
};
