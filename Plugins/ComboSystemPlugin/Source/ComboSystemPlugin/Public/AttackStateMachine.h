// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.h"
#include "AttackStateMachine.generated.h"

class UAttackIdleState;
class UAttackActionState;

/**
 * 
 */
UCLASS()
class COMBOSYSTEMPLUGIN_API UAttackStateMachine : public UStateMachine
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo StateMachine Attributes") TSubclassOf<UAttackIdleState> DefaultState;
	virtual void BeginPlay() override;

private:
	int32 InputButton;
	int32 InputState;

	uint32 bHasAnimationStopped : 1;
	uint32 bShouldTransitionToNextMove : 1;

	UPROPERTY() float CurrentDamage;

public:
	UAttackStateMachine();

	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions") FORCEINLINE void SetInputButton(int32 Value) { InputButton = Value; }
	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions") FORCEINLINE void SetInputState(int32 Value) { InputState = Value; }

	FORCEINLINE int32 GetInputButton() const { return InputButton; }
	FORCEINLINE int32 GetInputState() const { return InputState; }

	FORCEINLINE bool HasAnimationStopped() const { return bHasAnimationStopped; }
	FORCEINLINE bool ShouldTransitionToNextMove() const { return bShouldTransitionToNextMove; }

	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions") void SetShouldTransitionToNextMove(bool Value) { bShouldTransitionToNextMove = Value; }
	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions") void SetHasAnimationStopped(bool Value) { bHasAnimationStopped = Value; }

	FORCEINLINE TSubclassOf<UAttackIdleState> GetDefaultState() const { return DefaultState; }
	FORCEINLINE float GetCurrentStateDamage() const { return CurrentDamage; }
	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions")  void SetCurrentStateDamage(float NewDamage) { CurrentDamage = NewDamage; }

	void ResetFlags();


	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions") bool IsInAttackState();
	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions") bool IsInIdleState();
	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions") bool HasCompletedCombo();
	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions") bool IsKeyOnHold();
};
