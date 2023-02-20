// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatEmUpOw/AITasks/BTTask_ClearBlackBoardValues.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackBoardValues::UBTTask_ClearBlackBoardValues()
{
	NodeName = TEXT("Clear All Blackboard Values");
}

EBTNodeResult::Type UBTTask_ClearBlackBoardValues::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp,NodeMemory);

	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	
	return EBTNodeResult::Succeeded;
}
