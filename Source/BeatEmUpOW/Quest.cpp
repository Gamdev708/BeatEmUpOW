// Fill out your copyright notice in the Description page of Project Settings.

#include "BeatEmUpOW.h"
#include "Quest.h"

UQuestStatus::UQuestStatus() {
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}
 
void UQuestStatus::BeginPlay() {
	Super::BeginPlay();
}

void UQuestStatus::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

	Super::TickComponent(DeltaTime, TickType,  ThisTickFunction);
}

void UQuestStatus::UpdateQuests(USM_InputAtom* QuestActivity) {
	
	TArray<int32> RecentlyCompletedQuests;
	//Update the master list of everything we're ever done
	QuestActivities.Add(QuestActivity);

	//Update indiviual quests (if they care about this actvity) and see if they are complete
	for (int32 i = QuestList.Num()-1; i >= 0; --i)
	{
		if (QuestList[i].UpdateQuest(this,QuestActivity))
		{
			RecentlyCompletedQuests.Add(i);
			//(QuestList[i].QuestProgress == EQuestCompletion::EQC_Succeeded) ? TEXT("Suceeded") : TEXT("Failed");
		}
	}

	//Process completed quests after upadating all quests
	//This way a completed quest can't inject out of order input atom into other quests
	for (int32 i = RecentlyCompletedQuests.Num() - 1; i >= 0; --i)
	{
		FQuestInProgress& QIP = QuestList[RecentlyCompletedQuests[i]];
		if (QIP.QuestProgress==EQuestCompletion::EQC_Succeeded)
		{
			QIP.Quest->OnSuceeded(this);
		}
		else
		{
			QIP.Quest->OnFailed(this);
		}
		RecentlyCompletedQuests.RemoveAtSwap(i);
	}
}
bool UQuestStatus::BeginQuest(const UQuest* Quest) {
	for (FQuestInProgress& QIP:QuestList)
	{
		if (QIP.Quest==Quest)
		{
			if (QIP.QuestProgress==EQuestCompletion::EQC_NotStarted)
			{
				UE_LOG(LogTemp, Warning, TEXT("Changing quest \"%s\" to started Status"), *QIP.Quest->QuestName.ToString());
				QIP.QuestProgress = EQuestCompletion::EQC_Started;
				return true;
			}
			UE_LOG(LogTemp, Warning, TEXT("Quest \"%s\" is already in the list"), *QIP.Quest->QuestName.ToString());
			return false;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Adding Quests \"%s\" to the list and starting it"), *Quest->QuestName.ToString());
	QuestList.Add(FQuestInProgress::NewQuestInProgress(Quest));
	return true;
}

void UQuest::OnSuceeded(UQuestStatus* QuestStatus)const {
	UE_LOG(LogTemp, Warning, TEXT("Quest \"%s\" Succeeded!"), *QuestName.ToString());
}
void UQuest::OnFailed(UQuestStatus* QuestStatus) const {
	UE_LOG(LogTemp, Warning, TEXT("Quest \"%s\" Failed!"), *QuestName.ToString());
}


void UQuestWithResult::OnSuceeded(UQuestStatus* QuestStatus)const {	
	Super::OnSuceeded(QuestStatus);
	for (UQuest* SuccessQuest: SuccessQuests)
	{
		QuestStatus->BeginQuest(SuccessQuest);
	}
	for (int32 i = 0; i < SuccessInputs.Num(); ++i)
	{
		QuestStatus->UpdateQuests(SuccessInputs[i]);
	}
}
void UQuestWithResult::OnFailed(UQuestStatus* QuestStatus) const {
	Super::OnFailed(QuestStatus);
	for (UQuest* FailureQuest : FailureQuests)
	{
		QuestStatus->BeginQuest(FailureQuest);
	}
	for (int32 i = 0; i < FailureInputs.Num(); ++i)
	{
		QuestStatus->UpdateQuests(FailureInputs[i]);
	}
}

