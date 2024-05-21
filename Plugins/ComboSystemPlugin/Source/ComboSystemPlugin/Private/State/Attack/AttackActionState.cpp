// Fill out your copyright notice in the Description page of Project Settings.


#include "State/Attack/AttackActionState.h"

#include "AttackStateMachine.h"
#include "State/Attack/AttackIdleState.h"

void UAttackActionState::Enter()
{
	Super::Enter();
}

void UAttackActionState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	if (bIsFinal)
	{
		if (AttackStateMachine->HasAnimationStopped())
		{
			Reset();
		}
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UAttackActionState::Reset, WaitDuration);
	}
	EInputButtons EnteredInput = static_cast<EInputButtons>(AttackStateMachine->GetInputButton());
	EButtonState EnteredState = static_cast<EButtonState>(AttackStateMachine->GetInputState());

	if (EnteredInput == EInputButtons::None || EnteredState == EButtonState::None) { return; }


	if (!HandleInput(EnteredInput, EnteredState))
	{
		Reset();
		/*if (AttackStateMachine->HasAnimationStopped())
		{
			UE_LOG(LogTemp, Warning, TEXT("Input Wrong"));
			Reset();
		}*/
		return;
	}

	

	UAttackActionState* NextAction = nullptr;
	switch (EnteredInput)
	{
		
	case EInputButtons::LightAttack:
		switch (EnteredState)
		{
		case EButtonState::Pressed:
			if (!ensureAlwaysMsgf(LightAttackPress, TEXT("The Next Move LightAttackPress is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
			{
				SwitchToIdleState();
				return;
			}
			NextAction = NewObject<UAttackActionState>(AttackStateMachine, LightAttackPress);
			break;
		case EButtonState::Hold:
			if (!ensureAlwaysMsgf(LightAttackHold, TEXT("The Next Move is LightAttackHold Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
			{
				SwitchToIdleState();
				return;
			}
			NextAction = NewObject<UAttackActionState>(AttackStateMachine, LightAttackHold);
			break;
		case EButtonState::Tap:
			if (!ensureAlwaysMsgf(LightAttackTap, TEXT("The Next Move is LightAttackTap Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
			{
				SwitchToIdleState();
				return;
			}
			NextAction = NewObject<UAttackActionState>(AttackStateMachine, LightAttackTap);
			break;
		}
		break;
	case EInputButtons::HeavyAttack:
		switch (EnteredState)
		{
		case EButtonState::Pressed:
			if (!ensureAlwaysMsgf(HeavyAttackPress, TEXT("The Next Move HeavyAttackPress is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
			{
				SwitchToIdleState();
				return;
			}
			NextAction = NewObject<UAttackActionState>(AttackStateMachine, HeavyAttackPress);
			break;
		case EButtonState::Hold:
			if (!ensureAlwaysMsgf(HeavyAttackHold, TEXT("The Next Move HeavyAttackHold is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
			{
				SwitchToIdleState();
				return;
			}
			NextAction = NewObject<UAttackActionState>(AttackStateMachine, HeavyAttackHold);
			break;
		case EButtonState::Tap:
			if (!ensureAlwaysMsgf(HeavyAttackTap, TEXT("The Next Move HeavyAttackTap is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
			{
				SwitchToIdleState();
				return;
			}
			NextAction = NewObject<UAttackActionState>(AttackStateMachine, HeavyAttackTap);
			break;
		}
		break;
	}
	SwitchToNextActionState(NextAction);
}

void UAttackActionState::Exit()
{
	Super::Exit();
	AttackStateMachine->SetInputButton(0);
	AttackStateMachine->SetInputState(0);
	AttackStateMachine->ResetFlags();
}

void UAttackActionState::SwitchToNextActionState(UAttackActionState* NextAction) const
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	NextAction->Initialize(AttackStateMachine);
	AttackStateMachine->SwitchState(NextAction);
}

void UAttackActionState::SwitchToIdleState() const
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	UAttackIdleState* IdleState = NewObject<UAttackIdleState>(AttackStateMachine, AttackStateMachine->GetDefaultState());
	IdleState->Initialize(AttackStateMachine);
	AttackStateMachine->SwitchState(IdleState);
}

void UAttackActionState::Reset()
{
	SwitchToIdleState();
}
