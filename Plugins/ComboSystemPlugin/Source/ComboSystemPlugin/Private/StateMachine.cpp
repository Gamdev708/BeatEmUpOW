// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

#include "State/State.h"

// Sets default values for this component's properties
UStateMachine::UStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bHasStateChanged = false;

	// ...
}


// Called when the game starts
void UStateMachine::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState != nullptr)
	{
		CurrentState->Update(DeltaTime);
		//UE_LOG(LogTemp, Warning, TEXT("State:%s"), *GetNameSafe(CurrentState));
	}
	// ...
}

void UStateMachine::SwitchState(UState* State)
{
	bHasStateChanged = true;
	if (CurrentState) { CurrentState->Exit(); }
	CurrentState = State;
	if (CurrentState) { CurrentState->Enter(); }
}
