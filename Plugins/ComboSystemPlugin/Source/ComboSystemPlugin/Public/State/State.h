// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class COMBOSYSTEMPLUGIN_API UState : public UObject
{
	GENERATED_BODY()

public:
	virtual void Enter() PURE_VIRTUAL(UState, );
	virtual void Update(float DeltaTime) PURE_VIRTUAL(UState, );
	virtual void Exit() PURE_VIRTUAL(UState, );
};
