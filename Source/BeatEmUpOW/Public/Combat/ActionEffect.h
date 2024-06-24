// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Action.h"
#include "ActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API UActionEffect : public UAction
{
	GENERATED_BODY()

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect") float Duration;
	/*Time between ticks to apply*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect") float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effects") void ExecutePeriodicEffect(AActor* Instigator);
	UActionEffect();
};
