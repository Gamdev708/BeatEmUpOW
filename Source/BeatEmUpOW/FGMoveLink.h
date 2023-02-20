// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FGState.h"
#include "SM_State.h"
#include "FGMoveLink.generated.h"

class AMainCharacter;
class UFGMove;
/**
 * 
 */
UCLASS()
class BEATEMUPOW_API UFGMoveLink : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FStateMachineResult TryLinks(const AMainCharacter* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex = 0, int32 RemainingSteps = -1);

	UPROPERTY(EditAnywhere) UFGMove* Move;

	//Set of Windows(in seconds)when this link can be used
	UPROPERTY(EditAnywhere) TArray<FVector2D> CancelWindows;
	//Clears input buffer when using this link
	UPROPERTY(EditAnywhere) uint32 bClearInput : 1;
	//Sequence of inputs used to trigger this move. If not NULL, will overrride move's own input starts
	UPROPERTY(EditAnywhere) UFGState* InputStateMachine;
};
