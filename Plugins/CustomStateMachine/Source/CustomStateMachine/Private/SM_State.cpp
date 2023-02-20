// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_State.h"

USM_State::USM_State()
{
	bLoopByDefault = true;
}


FStateMachineResult USM_State::LoopState(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps)
{
	//By Default , Increase DataIndex by 1 and decrease Remainigsteps by 1.
	return RunState(RefObject, DataSource, DataIndex + 1, RemainingSteps - 1);
}


USM_State* USM_Branch::TryBranch(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32& OutDataIndex)
{
	OutDataIndex = DataIndex + 1;
	if (DataSource.IsValidIndex(DataIndex) && AcceptedInputs.Contains(DataSource[DataIndex]))
	{
		return bReverseInputTest ? nullptr : DestinationState;
	}
	return  bReverseInputTest ? DestinationState : nullptr;
}

FStateMachineResult USM_State::RunState(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps)
{
	bool bMustEndNow = (bTerminateImmediately || !DataSource.IsValidIndex(DataIndex));

	//If we're still running,see where our branches lead.
	if (RemainingSteps && !bMustEndNow)
	{
		USM_State* DestinationState = nullptr;
		int32 DestinationDataIndex = DataIndex;
		for (int32 i = 0; i < InstancedBranches.Num(); i++)
		{
			//This could be a check.There shouldn't be a null branch in the list
			if (InstancedBranches[i])
			{
				DestinationState = InstancedBranches[i]->TryBranch(RefObject, DataSource, DataIndex, DestinationDataIndex);
				if (DestinationState)
				{
					return DestinationState->RunState(RefObject, DataSource, DestinationDataIndex, RemainingSteps - 1);
				}
			}
		}
		for (int32 i = 0; i < SharedBranches.Num(); i++)
		{
			//This could be a check.There shouldn't be a null branch in the list
			if (SharedBranches[i])
			{
				DestinationState = SharedBranches[i]->TryBranch(RefObject, DataSource, DataIndex, DestinationDataIndex);
				if (DestinationState)
				{
					return DestinationState->RunState(RefObject, DataSource, DestinationDataIndex, RemainingSteps - 1);
				}
			}
		}
		//We didn't find any accepteable input branchs, so we must end on this state unless we're told to loop.
		if (bLoopByDefault)
		{
			return LoopState(RefObject, DataSource, DataIndex, RemainingSteps);
		}
		bMustEndNow = true;
	}

	FStateMachineResult SMR;
	SMR.FinalState = this;
	SMR.DataIndex = DataIndex;
	SMR.CompeletionType = bMustEndNow ? CompeletionType : EStateMachineCompletionType::OutofSteps;
	return SMR;
}

USM_State* USM_BranchBase::TryBranch(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32& OutDataIndex)
{
	OutDataIndex = DataIndex;
	return DestinationState;
}
