// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SM_State.h"
#include "FGState.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API UFGState : public USM_State
{
	GENERATED_BODY()
	
protected:
	virtual FStateMachineResult LoopState(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps) override;

};
