// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatEmUpOW/AITasks/BTService_UpdateCombatRange.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

UBTService_UpdateCombatRange::UBTService_UpdateCombatRange()
{
}

void UBTService_UpdateCombatRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//AAIController* AIController = OwnerComp.GetAIOwner()->GetActorLocation();
	//if (AIController == nullptr)
	//{
	//	return;
	//}
}
