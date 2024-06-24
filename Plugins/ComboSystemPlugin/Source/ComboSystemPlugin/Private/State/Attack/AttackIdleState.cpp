// Fill out your copyright notice in the Description page of Project Settings.


#include "State/Attack/AttackIdleState.h"

#include "AttackStateMachine.h"
#include "State/Attack/AttackActionState.h"

void UAttackIdleState::Enter()
{
	Super::Enter();
}

void UAttackIdleState::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	EInputButtons EnteredInput = static_cast<EInputButtons>(AttackStateMachine->GetInputButton());
	EButtonState EnteredState = static_cast<EButtonState>(AttackStateMachine->GetInputState());
	if (EnteredInput == EInputButtons::None || EnteredState == EButtonState::None) { return; }

	UAttackActionState* NextAction = nullptr;

	if (HandleInput(EnteredInput, EnteredState))
	{
		switch (EnteredInput)
		{
		case EInputButtons::LightAttack:
			switch (EnteredState)
			{
			case EButtonState::Pressed:
				if (!ensureAlwaysMsgf(LightAttackPress, TEXT("The Next Move LightAttackPress is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, LightAttackPress);
				break;
			case EButtonState::Hold:
				if (!ensureAlwaysMsgf(LightAttackHold, TEXT("The Next Move is LightAttackHold Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, LightAttackHold);
				break;
			case EButtonState::Tap:
				if (!ensureAlwaysMsgf(LightAttackTap, TEXT("The Next Move is LightAttackTap Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, LightAttackTap);
				break;
			}
			break;

		case EInputButtons::MediumAttack:
			switch (EnteredState)
			{
			case EButtonState::Pressed:
				if (!ensureAlwaysMsgf(MediumAttackPress, TEXT("The Next Move MediumAttackPress is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, MediumAttackPress);
				break;
			case EButtonState::Hold:
				if (!ensureAlwaysMsgf(MediumAttackHold, TEXT("The Next Move MediumAttackHold is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, MediumAttackHold);
				break;
			case EButtonState::Tap:
				if (!ensureAlwaysMsgf(MediumAttackTap, TEXT("The Next Move MediumAttackTap is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, MediumAttackTap);
				break;
			}
			break;

		case EInputButtons::HeavyAttack:
			switch (EnteredState)
			{
			case EButtonState::Pressed:
				if (!ensureAlwaysMsgf(HeavyAttackPress, TEXT("The Next Move HeavyAttackPress is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, HeavyAttackPress);
				break;
			case EButtonState::Hold:
				if (!ensureAlwaysMsgf(HeavyAttackHold, TEXT("The Next Move HeavyAttackHold is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, HeavyAttackHold);
				break;
			case EButtonState::Tap:
				if (!ensureAlwaysMsgf(HeavyAttackTap, TEXT("The Next Move HeavyAttackTap is Not SET!! Please make sure you have attached the related move to the given input params. AUTO switching to Idle Now!")))
				{
					return;
				}
				NextAction = NewObject<UAttackActionState>(AttackStateMachine, HeavyAttackTap);
				break;
			}
			break;
		}

		NextAction->Initialize(AttackStateMachine);
		//AttackStateMachine->SetCurrentStateDamage(NextAction->GetDamage());
		AttackStateMachine->SwitchState(NextAction);
	}

}

void UAttackIdleState::Exit()
{
	Super::Exit();
	AttackStateMachine->SetInputButton(0);
	AttackStateMachine->SetInputState(0);
}
