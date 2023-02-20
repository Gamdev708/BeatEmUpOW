// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable, Category = "Animation Properties") void UpdateAnimationProperites();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") class APawn* Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement") class AEnemyCharacter* Enemy;
	
};
