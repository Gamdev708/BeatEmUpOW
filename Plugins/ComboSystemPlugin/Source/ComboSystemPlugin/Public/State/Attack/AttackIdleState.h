// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/Attack/AttackBaseState.h"
#include "AttackIdleState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class COMBOSYSTEMPLUGIN_API UAttackIdleState : public UAttackBaseState
{
	GENERATED_BODY()

public:
	virtual void Enter() override;
	virtual void Update(float DeltaTime) override;
	virtual void Exit() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Light") TSubclassOf<UAttackActionState> LightAttackPress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Light") TSubclassOf<UAttackActionState> LightAttackHold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Light") TSubclassOf<UAttackActionState> LightAttackTap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Heavy") TSubclassOf<UAttackActionState> HeavyAttackPress;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Heavy") TSubclassOf<UAttackActionState> HeavyAttackHold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Path|Heavy") TSubclassOf<UAttackActionState> HeavyAttackTap;

};
