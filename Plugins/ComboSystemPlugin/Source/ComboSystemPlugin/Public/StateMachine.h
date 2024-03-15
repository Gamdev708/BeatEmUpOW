// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"

class UState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBOSYSTEMPLUGIN_API UStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachine();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY() UState* CurrentState;
	UState* NewState;
	uint32 bHasStateChanged : 1;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SwitchState(UState* State);
	UFUNCTION(BlueprintCallable, Category = "Combo StateMachine Functions")  FORCEINLINE bool HasStateChanged()const { return bHasStateChanged; }
};






