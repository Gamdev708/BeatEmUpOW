// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataAsset.h"
#include "SM_State.h"
#include "Quest.generated.h"


UENUM()
enum class EQuestCompletion :uint8
{
	EQC_NotStarted,
	EQC_Started,
	EQC_Succeeded,
	EQC_Failed
};

/**
 * 
 */
UCLASS()
class BEATEMUPOW_API UQuest : public UDataAsset
{
	GENERATED_BODY()
public:
	//The name of the Quest
	UPROPERTY(EditAnywhere) FText QuestName;
	//If this machine accepts our Quest Activites log. The Quest is successful
	UPROPERTY(EditAnywhere) USM_State* QuestStateMachine;
	//If true, the Inputlist is a blacklist,Otherwise it's a whitelist
	UPROPERTY(EditAnywhere) uint32 bInputBlackList : 1;
	// the blacklist/whitelist (depending on the bBlacklist) used ot filter InputAtoms this Quest recognizes
	UPROPERTY(EditAnywhere) TArray<USM_InputAtom*> InputList;

	virtual void OnSuceeded(class UQuestStatus* QuestStatus) const;
	virtual void OnFailed(class UQuestStatus* QuestStatus) const;
};

USTRUCT()
struct FQuestInProgress {
	GENERATED_BODY()

		//Quest data asset
		UPROPERTY(EditAnywhere) const UQuest* Quest;
		//Current Progress in the quest
		UPROPERTY(EditAnywhere) EQuestCompletion QuestProgress;
protected:
	//All input for this quest.Filtered by the quest's blacklist/whitelist
		UPROPERTY(EditAnywhere) TArray<USM_InputAtom*> QuestActivities;
public:
	//Returns true if the quest was completeted(success or failure) by the new activity
	bool UpdateQuest(const UObject* ObjectRef, USM_InputAtom* QuestActivity) {
		//Only log activity to valid,in-progress quests.Check the blacklist/Whitelist before logging
		if (Quest &&(QuestProgress== EQuestCompletion::EQC_Started)
			&&(Quest->bInputBlackList !=Quest->InputList.Contains(QuestActivity)))
		{
			FStateMachineResult QuestResult;
			QuestActivities.Add(QuestActivity);
			QuestResult = Quest->QuestStateMachine->RunState(ObjectRef, QuestActivities);
			switch (QuestResult.CompeletionType)
			{
			case EStateMachineCompletionType::Accepted:
				QuestProgress = EQuestCompletion::EQC_Succeeded;
				return true;
			case EStateMachineCompletionType::Rejected:
				QuestProgress = EQuestCompletion::EQC_Failed;
				return true;
			// case NotAccepted:// still in progress. No update needed
			default:
				break;
			}
		}
		return false;
	}

	static FQuestInProgress NewQuestInProgress(const UQuest* _Quest) {
		FQuestInProgress NewQIP;
		NewQIP.Quest = _Quest;
		NewQIP.QuestProgress = EQuestCompletion::EQC_Started;
		return NewQIP;
	}
};

UCLASS(ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class BEATEMUPOW_API UQuestStatus:public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets defualt values for this component's properties
	UQuestStatus();
	
	//Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Add to our quest Activity log! this also automatically checks to see if any unfinished quests are unifinished quests are now complete
	UFUNCTION(BlueprintCallable, Category = "Quests") void UpdateQuests(USM_InputAtom* QuestActivity);
	/** Add a new Quest-in-progress entry,or begin the quest provided if it's already on the list and hasn't being started*/
	UFUNCTION(BlueprintCallable, Category = "Quests") bool BeginQuest(const UQuest* Quest);

protected:
	//The master list of all quest-related things we've done
	UPROPERTY(EditAnywhere) TArray< USM_InputAtom*> QuestActivities;
	//The list of quests in out current game or area
	UPROPERTY(EditAnywhere) TArray<FQuestInProgress> QuestList;

};

UCLASS()
class BEATEMUPOW_API UQuestWithResult : public UQuest
{
	GENERATED_BODY()
public:
	virtual void OnSuceeded(class UQuestStatus* QuestStatus) const override;
	virtual void OnFailed(class UQuestStatus* QuestStatus) const override;

protected:
	//The quest in this list will go from NotStarted to Started if the current quest suceeds
	UPROPERTY(EditAnywhere) TArray<UQuest*> SuccessQuests;
	
	//Input Atom to add if the quest succeedes
	UPROPERTY(EditAnywhere) TArray<USM_InputAtom*> SuccessInputs;
	//The Quest in this list will go from NotStarted to Started if the current quest fails
	UPROPERTY(EditAnywhere) TArray<UQuest*> FailureQuests;
	//Input Atoms to add if the quest fails
	UPROPERTY(EditAnywhere) TArray<USM_InputAtom*> FailureInputs;
};