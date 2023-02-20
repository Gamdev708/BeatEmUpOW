// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatEmUpOW/Characters/MeleeAIController.h"
#include "BeatEmUpOW/Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BeatEmUpOW/Characters/EnemyCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void AMeleeAIController::BeginPlay()
{
	Super::BeginPlay();
	if (AIBehaviour != nullptr) {
		RunBehaviorTree(AIBehaviour);
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());

		//AEnemyCharacter * EnemyAI=Cast<AEnemyCharacter>(GetPawn());
		//if (EnemyAI == nullptr) { return; }

		//GetBlackboardComponent()->SetValueAsFloat(TEXT("Range"), EnemyAI->GetEnemyRange());
		//GetBlackboardComponent()->SetValueAsEnum(TEXT("EnemyType"),(uint8) EnemyAI->GetEnemyType());
	}
}

void AMeleeAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AMeleeAIController::IsDead() const
{
	/*AMainCharacter* ControllerChracter = Cast<AMainCharacter>(GetPawn());
	if (ControllerChracter != nullptr)
	{
		return ControllerChracter->IsDead();
	}*/
	return true;
}