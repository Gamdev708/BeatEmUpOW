// Fill out your copyright notice in the Description page of Project Settings.


#include "FGMove.h"
#include "FGMoveLink.h"

FFGMoveLinkToFollow UFGMove::TryLinks(const AMainCharacter* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps)
{
	FFGMoveLinkToFollow ResultLink;

	for (int32 i = 0; i < LinkedMoves.Num(); i++)
	{
		check(LinkedMoves[i]);
		ResultLink.SMR = LinkedMoves[i]->TryLinks(RefObject, DataSource, DataIndex, RemainingSteps);
		if (ResultLink.SMR.CompeletionType==EStateMachineCompletionType::Accepted)
		{
			ResultLink.Link = LinkedMoves[i];
			return ResultLink;
		}
	}
	return ResultLink;
}
