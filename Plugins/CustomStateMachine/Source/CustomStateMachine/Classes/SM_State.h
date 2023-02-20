// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SM_State.generated.h"

class USM_State;
class USM_Branch;

UENUM()
enum class EStateMachineCompletionType:uint8
{
	//Implict failure -this state is not marked as Accept
	NotAccepted,
	//Success - this state is an Accept State
	Accepted,
	//Explict Failure - this state is specifically marked as failure/not accept
	Rejected,
	//Our simulation ran out of steps while the machine still running
	OutofSteps UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct CUSTOMSTATEMACHINE_API FStateMachineResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY() EStateMachineCompletionType CompeletionType;
	UPROPERTY() USM_State* FinalState;
	UPROPERTY() int32 DataIndex;
};

UCLASS()
class CUSTOMSTATEMACHINE_API USM_InputAtom : public UDataAsset
{
	GENERATED_BODY()
public:
	//Dispaly value for this input atom,mainly for debugging purposes
	UPROPERTY(EditAnywhere) FName Description;
};

UCLASS(EditInlineNew)
class CUSTOMSTATEMACHINE_API USM_BranchBase:public UDataAsset
{
	GENERATED_BODY()
public:
	/**Returns DestinationState on sucess,NULL on failure, for subclasses
	OutDataIndex might be something other than 1m if a branch is made to consume multiple inputs*/
	virtual USM_State* TryBranch(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32& OutDataIndex);

protected:
	//State where we will go next if this branch is taken. IF null,this branch will be ignored.
	UPROPERTY(EditAnywhere) USM_State* DestinationState;

};

UCLASS(EditInlineNew)
class CUSTOMSTATEMACHINE_API USM_Branch:public USM_BranchBase
{
	GENERATED_BODY()
public:
	/**Returns DestinationState on sucess,NULL on failure, for subclasses
	OutDataIndex might be something other than 1m if a branch is made to consume multiple inputs*/
	virtual USM_State* TryBranch(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32& OutDataIndex) override;

protected:

	//If true, the of meaning of AcceptableInputs is reversed.
	UPROPERTY(EditAnywhere) uint32 bReverseInputTest : 1;
	//Accepted Inputs, The current input atom must be on this list
	UPROPERTY(EditAnywhere) TArray<USM_InputAtom*> AcceptedInputs;
};


/**
 * 
 */
UCLASS()
class CUSTOMSTATEMACHINE_API USM_State : public UDataAsset
{
	GENERATED_BODY()
public:
	USM_State();

	/**Attempt to run the state with input from the given DataSource object,
	Will start reading at DataIndex.
	Will decrement RemainingSteps and automatically fail after it hits 0.*/
	UFUNCTION(BlueprintCallable,Category="State Machine")
		virtual FStateMachineResult RunState(const UObject* RefObject,const TArray<USM_InputAtom*>& DataSource, int32 DataIndex=0, int32 RemainingSteps=-1);
protected:
	//Loop, used when input atom being process isn't recongnized
	virtual FStateMachineResult LoopState(const UObject* RefObject, const TArray<USM_InputAtom*>& DataSource, int32 DataIndex, int32 RemainingSteps);

	/**If input runs out on this state (or TeminiateImmediately is true),
	this is how the result will be interuptted*/
	UPROPERTY(EditAnywhere)	EStateMachineCompletionType CompeletionType;

	//If this is set ,this state will loop on itslef whenever an unhandled input atom is detected
	UPROPERTY(EditAnywhere)	uint32 bLoopByDefault : 1;

	/**A State machine run that enteres state will terminiate immediatly, regardless os whether or not there is more input data*/
	UPROPERTY(EditAnywhere)	uint32 bTerminateImmediately : 1;

	// Branches to other states. There are in priority order,so the first sucessful branch will be taken
	UPROPERTY(EditAnywhere,Instanced)TArray<USM_BranchBase*> InstancedBranches;
	// Branches to other states. There are in priority order,so the first sucessful branch will be taken. These run after InstancedBranches
	UPROPERTY(EditAnywhere)TArray<USM_BranchBase*> SharedBranches;
};
