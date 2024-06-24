// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackStateMachine.h"

#include "State/Attack/AttackActionState.h"
#include "State/Attack/AttackIdleState.h"

UAttackStateMachine::UAttackStateMachine()
{
	InputButton = 0;
	InputState = 0;
	CurrentDamage = 0;
}

void UAttackStateMachine::BeginPlay()
{
	Super::BeginPlay();

	UAttackIdleState* IdleState = NewObject<UAttackIdleState>(this, DefaultState);
	IdleState->Initialize(this);
	SwitchState(IdleState);
}

void UAttackStateMachine::ResetFlags()
{
	bHasAnimationStopped = false;
	bShouldTransitionToNextMove = false;
	CurrentDamage = 0;
}

bool UAttackStateMachine::IsInAttackState()
{
	if (CurrentState == nullptr) { return false; }
	return CurrentState->IsA(UAttackActionState::StaticClass());
}

bool UAttackStateMachine::IsInIdleState()
{
	if (CurrentState == nullptr) { return false; }
	return CurrentState->IsA(UAttackIdleState::StaticClass());
}

bool UAttackStateMachine::HasCompletedCombo()
{
	if (CurrentState == nullptr) { return false; }

	UAttackActionState* State = Cast<UAttackActionState>(CurrentState);
	if (State == nullptr)
	{
		return false;
	}
	return  State->IsFinal();
}
