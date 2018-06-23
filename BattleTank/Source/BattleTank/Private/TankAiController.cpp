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
	if (ControlledTank == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AiController Failed to find tank"));
	}

	ATank* PlayerTank = GetPlayerTank();
	if (PlayerTank == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AiController Failed to find PlayerTank"));
	}
}

void ATankAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// TODO: move towards the player
	
	AimTowardsPlayerTank();

	// TODO: fire at player when ready
}

ATank* ATankAiController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAiController::GetPlayerTank() const
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	/*if (PlayerPawn == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Failed to find Player Tank"));
		return nullptr;
	}*/

	return Cast<ATank>(PlayerPawn);	
}

void ATankAiController::AimTowardsPlayerTank() const
{
	ATank* PlayerTank = GetPlayerTank();
	if (PlayerTank) {
		GetControlledTank()->AimAt(PlayerTank->GetActorLocation());
	}
}

