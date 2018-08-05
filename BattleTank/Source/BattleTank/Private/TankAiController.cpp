// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAiController.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankAiController::BeginPlay()
{
	Super::BeginPlay();	

	// try to get the tank
	ATank* ControlledTank = GetControlledTank();
	if (!ensure(ControlledTank)) {
		UE_LOG(LogTemp, Error, TEXT("AiController Failed to find tank"));
	}

	ATank* PlayerTank = GetPlayerTank();
	if (!ensure(PlayerTank)) {
		UE_LOG(LogTemp, Error, TEXT("AiController Failed to find PlayerTank"));
	}
}

void ATankAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ATank* PlayerTank = GetPlayerTank();
	if (PlayerTank) {
		ATank* ControlledTank = GetControlledTank();

		// move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim towards player tank
		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		// fire at player when ready
		ControlledTank->Fire();
	}	
}

ATank* ATankAiController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAiController::GetPlayerTank() const
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerPawn)) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find Player Tank"));
		return nullptr;
	}

	return Cast<ATank>(PlayerPawn);	
}
