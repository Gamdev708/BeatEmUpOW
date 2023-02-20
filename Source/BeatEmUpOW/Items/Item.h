// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class BEATEMUPOW_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particle") bool bWeaponParticles;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")class USoundCue* OnEquipSound;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound") USoundCue* SwingSound;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh") class USkeletalMeshComponent* SkeletalMesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat") class UBoxComponent* CombatCollision;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat") float Damage;

	//UFUNCTION() void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION() void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//void Equip(class AMainCharacter* Char);

};
