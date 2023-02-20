// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatEmUpOw/AITasks/BTTaskNode_Attack.h"
#include "BeatEmUpOW/Characters/EnemyCharacter.h"
#include "AIController.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController= OwnerComp.GetAIOwner();
	if (AIController== nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	AEnemyCharacter* EnemyChracter =Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (EnemyChracter == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	EnemyChracter->Attack();

	return EBTNodeResult::Succeeded;
}
