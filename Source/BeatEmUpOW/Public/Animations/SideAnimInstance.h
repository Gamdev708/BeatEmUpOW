// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SideAnimInstance.generated.h"

class ABaseSideFighterCharacter;

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API USideAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable, Category = "Animation Properties") void UpdateAnimationProperties();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float AirSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") bool bIsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") bool bShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") bool bIsCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") class APawn* Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") TObjectPtr<ABaseSideFighterCharacter> Main;

};
