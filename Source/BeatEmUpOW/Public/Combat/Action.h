// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Action.generated.h"


class UWorld;
class USActionComponent;


USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	TObjectPtr<AActor> Instigator;
};


/**
 * 
 */
UCLASS()
class BEATEMUPOW_API UAction : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	TObjectPtr<UActionComponent> ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionComponent* GetOwningComponent() const;

	///* Tags added to owning actor when activated, removed when action stops */
	//UPROPERTY(EditDefaultsOnly, Category = "Tags")
	//FGameplayTagContainer GrantsTags;

	///* Action can only start if OwningActor has none of these Tags applied */
	//UPROPERTY(EditDefaultsOnly, Category = "Tags")
	//FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionRepData RepData;

	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION()
	void OnRep_RepData();

public:

	void Initialize(UActionComponent* NewActionComp);

	/* Start immediately when added to an action component */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	
};
