// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FighterGameModeBase.generated.h"

class ABaseSideFighterCharacter;

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API AFighterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFighterGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players") TObjectPtr<ABaseSideFighterCharacter> Player1Ref;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players") TObjectPtr<ABaseSideFighterCharacter> Player2Ref;

};
