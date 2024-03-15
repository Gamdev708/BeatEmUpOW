// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/Attack/AttackBaseState.h"
#include "AttackActionState.generated.h"

/**
 * 
 */
UCLASS()
class COMBOSYSTEMPLUGIN_API UAttackActionState : public UAttackBaseState
{
	GENERATED_BODY()

public:
	virtual void Enter() override;
	UFUNCTION(BlueprintCallable, Category = "Combo State Functions") void Reset();
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

	/*FORCEINLINE EInputButtons GetInputButton() { return RequiredInput; }
	FORCEINLINE EButtonState GetButtonState() { return Buttonstate; }*/
	UFUNCTION(BlueprintCallable,Category="Action State Settings")  void AddDuration(float time) { WaitDuration += time; }
	UFUNCTION(BlueprintCallable,Category="Action State Settings") FORCEINLINE bool IsFinal() { return bIsFinal; }
	UFUNCTION(BlueprintCallable,Category="Action State Settings") void SetIsFinal(bool Value) { bIsFinal = Value; }
private:
	void SwitchToIdleState() const;
	void SwitchToNextActionState(UAttackActionState* NextAction) const;

	FTimerHandle ComboTimerHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo Settings") FName MoveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Light", meta = (EditCondition = "!bIsFinal", EditConditionHides)) TSubclassOf<UAttackActionState> LightAttackPress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Light", meta = (EditCondition = "!bIsFinal", EditConditionHides)) TSubclassOf<UAttackActionState> LightAttackHold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Light", meta = (EditCondition = "!bIsFinal", EditConditionHides)) TSubclassOf<UAttackActionState> LightAttackTap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Heavy", meta = (EditCondition = "!bIsFinal", EditConditionHides)) TSubclassOf<UAttackActionState> HeavyAttackPress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Heavy", meta = (EditCondition = "!bIsFinal", EditConditionHides)) TSubclassOf<UAttackActionState> HeavyAttackHold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Heavy", meta = (EditCondition = "!bIsFinal", EditConditionHides)) TSubclassOf<UAttackActionState> HeavyAttackTap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Settings") bool bIsFinal;
	/* The duration to wait before executing the next move. This property is always editable */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo Settings", meta = (EditCondition = "bWaitforInput", EditConditionHides)) float WaitDuration;

};
