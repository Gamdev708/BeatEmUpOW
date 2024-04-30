// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActionComponent.h"
#include "Combat/Action.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

//DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}	
}

void UActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}


// Called every frame
void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// Draw All Actions
// 	for (USAction* Action : Actions)
// 	{
// 		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
// 		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
// 
// 		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
// 	}
}


void UActionComponent::AddAction(AActor* Instigator, TSubclassOf<UAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UAction* NewAction = NewObject<UAction>(GetOwner(), ActionClass);

	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UActionComponent::RemoveAction(UAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

UAction* UActionComponent::GetAction(TSubclassOf<UAction> ActionClass) const
{
	for (UAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

bool UActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	//SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for (UAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			// Is Client?
			/*if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}*/

			// Bookmark for Unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *GetNameSafe(Action));

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{

	for (UAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				//// Is Client?
				//if (!GetOwner()->HasAuthority())
				//{
				//	ServerStopAction(Instigator, ActionName);
				//}

				Action->StopAction(Instigator);
				return true;
			}
		}
	}


	return false;
}

void UActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActionComponent, Actions);
}