// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatEmUpOw/AITasks/BTService_CheckEnemyType.h"
#include "BeatEmUpOW/Characters/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_CheckEnemyType::UBTService_CheckEnemyType()
{
	NodeName = TEXT("Check Type");
}

void UBTService_CheckEnemyType::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyCharacter* EnemyChracter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (EnemyChracter == nullptr)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)EnemyChracter->GetEnemyType());

}
