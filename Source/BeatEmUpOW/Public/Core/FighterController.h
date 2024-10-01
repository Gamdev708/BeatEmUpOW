// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "FighterController.generated.h"

class UInputAction;
class UInputMappingContext;
class UAttackStateMachine;
class UActionComponent;
class ABaseSideFighterCharacter;

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API AFighterController : public APlayerController
{
	GENERATED_BODY()



protected:
	/** MappingContext */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* DuckAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* DashAction;
	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* LightAttackPunchAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MediumAttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* HeavyAttackAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true")) TObjectPtr<UAttackStateMachine> AttackStateMachineComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true")) TObjectPtr<UActionComponent> ActionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Actors", meta = (AllowPrivateAccess = "true")) TObjectPtr<ABaseSideFighterCharacter> PlayerCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actors", meta = (AllowPrivateAccess = "true")) TObjectPtr<ABaseSideFighterCharacter> OtherFighter;

	 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input|Timers", meta = (AllowPrivateAccess = "true")) float KeyHoldPressTime;
	 

public:
    AFighterController();
    virtual void Tick(float DeltaSeconds) override;

protected:
    // To add mapping context
    virtual void BeginPlay();

    virtual void SetupInputComponent() override;

private:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void DoCrouch();
	void DoUncrouch();
	void DoDash();
	void AttackLight();
	void AttackMedium();
	void AttackHeavyPressed();
	void AttackHeavyTrigger();
	void AttackHeavyReleased();
	void KeyHoldTimerFunc();


	FTimerHandle KeyHoldPressTimerHandle;

public:
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetOtherFighter(ABaseSideFighterCharacter* Fighter) { this->OtherFighter = Fighter; }
};
