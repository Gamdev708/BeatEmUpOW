// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseSideAIController.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API ABaseSideAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseSideAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	APawn* PlayerPawn;
	UPROPERTY(EditAnywhere, Category = "AI") float AcceptanceRadius = 100.f; // Distance at which the AI stops following the player

public:
	UFUNCTION(BlueprintCallable) FORCEINLINE void SetPlayer(APawn* Player) { PlayerPawn = Player; }
	
};
