// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/SideAnimInstance.h"
#include "Characters/BaseSideFighterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../DebugHeader.h"
void USideAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Main = Cast<ABaseSideFighterCharacter>(Pawn);
		}
	}
}

void USideAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) { Pawn = TryGetPawnOwner(); }

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		//Debug::Print(Speed.ToCompactString());
		FVector LateralSpeed = FVector(0, Speed.Y, Speed.Z);
		MovementSpeed = Speed.Y;
		if (Speed.X==0)
		{
			bShouldMove = false;
		}



		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
		bIsCrouch = Pawn->GetMovementComponent()->IsCrouching();
		/*if (Main == nullptr)
		{
			Main = Cast<ABaseSideFighterCharacter>(Pawn);
			bIsCrouch = Main->GetCharacterMovement()->IsCrouching();
		}*/


	}
}
