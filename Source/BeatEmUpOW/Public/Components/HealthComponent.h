// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UHealthComponent*, OwningComp, float, NewValue, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEATEMUPOW_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes") static UHealthComponent* GetHealthComp(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))	static bool IsActorAlive(AActor* Actor);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes") float HealthMax;
	


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attributes") bool Kill(AActor* InstigatorActor);
	UFUNCTION(BlueprintCallable, Category = "Attributes") bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable, Category = "Attributes")	FOnHealthChanged OnHealthChanged;
	FORCEINLINE float const GetMaxHealth() { return HealthMax; }
	FORCEINLINE float const GetHealth() { return Health; }
	FORCEINLINE bool const IsAlive() { return Health > 0.0f; }
	FORCEINLINE bool const IsFullHealth()  { return Health == HealthMax; }
		
};


/// candidate salary infoview line number 83, array index is empty