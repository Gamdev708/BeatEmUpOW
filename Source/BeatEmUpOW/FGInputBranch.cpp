// Fill out your copyright notice in the Description page of Project Settings.


#include "FGInputBranch.h"

USM_State* UFGInputBranch::TryBranch(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32& OutDataIndex)
{
	OutDataIndex = DataIndex;
	return nullptr;
}
