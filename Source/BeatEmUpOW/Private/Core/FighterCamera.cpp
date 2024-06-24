// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/FighterCamera.h"

// Sets default values
AFighterCamera::AFighterCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFighterCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFighterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (PlayerOne && PlayerTwo)
    {
        FVector PlayerOneLocation = PlayerOne->GetActorLocation();
        FVector PlayerTwoLocation = PlayerTwo->GetActorLocation();

        // Midpoint between PlayerOne and PlayerTwo
        FVector Midpoint = (PlayerOneLocation + PlayerTwoLocation) / 2.0f;

        // Absolute difference in X and Y coordinates
        float AbsDiffX = FMath::Abs(PlayerOneLocation.X - PlayerTwoLocation.X);
        float AbsDiffY = FMath::Abs(PlayerOneLocation.Y - PlayerTwoLocation.Y);

        // Perform checks
        bool IsDiffXGreaterThan500 = (AbsDiffX > 500.0f);
        bool IsDiffYLessThanOrEqual2000 = (AbsDiffY <= 2000.0f);

        //// Additional logic based on checks
        //if (IsDiffXGreaterThan500 && IsDiffYLessThanOrEqual2000)
        //{
        //    // Implement the logic that should run when both conditions are true
        //    FVector AdjustedLocation = Midpoint;

        //    // Example adjustment (similar to the logic in your blueprint)
        //    AdjustedLocation.Z = FMath::Max(AdjustedLocation.Z, 150.0f);

        //    // Set new location or perform other operations
        //    PlayerOne->SetActorLocation(AdjustedLocation);
        //}
    }
}

