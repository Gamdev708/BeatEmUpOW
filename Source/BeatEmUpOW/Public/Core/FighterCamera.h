// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FighterCamera.generated.h"

UCLASS()
class BEATEMUPOW_API AFighterCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFighterCamera();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)	AActor* PlayerOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	AActor* PlayerTwo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
