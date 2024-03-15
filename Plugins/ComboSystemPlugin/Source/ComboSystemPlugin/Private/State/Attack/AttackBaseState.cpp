// Fill out your copyright notice in the Description page of Project Settings.


#include "State/Attack/AttackBaseState.h"

UAttackBaseState::UAttackBaseState()
{
}

void UAttackBaseState::Enter()
{
	this->OnEnter();
}

void UAttackBaseState::Update(float DeltaTime)
{
	this->OnUpdate(DeltaTime);
}

void UAttackBaseState::Exit()
{
	this->OnExit();
}

bool UAttackBaseState::HandleInput(EInputButtons EnteredInput, EButtonState EnteredState)
{
	if (EnteredState == EButtonState::Invalid || EnteredInput == EInputButtons::Invalid) { return false; }
	if (!RequiredInputs.Contains(EnteredInput) || !RequiredStates.Contains(EnteredState)) { return false; }

	return true;
}
