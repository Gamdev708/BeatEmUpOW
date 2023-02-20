// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SM_State.h"
#include "FGAtoms.generated.h"

/**
 * 
 */

UENUM(Blueprintable,Meta=(Bitflags))
enum class EFGInputDirections :uint8 {
	Forward,
	Left,
	Right,
	Backward,
	ForwardBack,
	BackForward,
	Neutral
};

UENUM()
enum class EFGButtonState :uint8 {
	Up,
	JustPressed,
	HeldDown,
	/*HeldDownForTime,*/
	Count UMETA(Hidden)
};

UENUM(Blueprintable, Meta = (Bitflags))
enum class EFGInputButtons :uint8 {
	LightAttack,
	HeavyAttack,
	Count UMETA(Hidden)
};

UCLASS()
class BEATEMUPOW_API UFGInputDirectionalAtom : public USM_InputAtom {
	GENERATED_BODY()
public:
	//Which direction we're describing with this atom
	UPROPERTY(EditAnywhere) EFGInputDirections InputDirection;
};
UCLASS()
class BEATEMUPOW_API UFGButtonInputAtom : public USM_InputAtom {
	GENERATED_BODY()
public:
	//State of Button being described
	UPROPERTY(EditAnywhere) EFGButtonState ButtonState;
};
