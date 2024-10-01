// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseSideAIController.h"
#include <Characters/BaseSideFighterCharacter.h>
#include "../../../Plugins/ComboSystemPlugin/Source/ComboSystemPlugin/Public/AttackStateMachine.h"



ABaseSideAIController::ABaseSideAIController()
{
}

void ABaseSideAIController::BeginPlay()
{

}

void ABaseSideAIController::Tick(float DeltaTime)
{

	if (TObjectPtr<ABaseSideFighterCharacter> Char = Cast<ABaseSideFighterCharacter>(GetCharacter())) {
		PlayerPawn = Char->OtherFighter;
	}
	
	if (PlayerPawn)
	{

		float DistanceToPlayer = FVector::Dist(PlayerPawn->GetActorLocation(), GetPawn()->GetActorLocation());

		if (DistanceToPlayer > AcceptanceRadius)
		{
			MoveToActor(PlayerPawn, AcceptanceRadius);
		}
		else
		{
			StopMovement();
			/*if (TObjectPtr<ABaseSideFighterCharacter> Char = Cast<ABaseSideFighterCharacter>(GetCharacter())) 
			{

				Char->GetAttackStateMachineComponent()->SetInputState(1);
				Char->GetAttackStateMachineComponent()->SetInputButton(1);
			}*/
		}
	}
}
