// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckEnemyType.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API UBTService_CheckEnemyType : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CheckEnemyType();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
