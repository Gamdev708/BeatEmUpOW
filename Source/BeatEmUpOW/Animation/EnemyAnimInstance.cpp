// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatEmUpOW/Animation/EnemyAnimInstance.h"
#include "BeatEmUpOW/Characters/EnemyCharacter.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AEnemyCharacter>(Pawn);
		}

	}
}

void UEnemyAnimInstance::UpdateAnimationProperites()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AEnemyCharacter>(Pawn);
		}

	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}
}
