// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("PlayerController Begin Play"));

	// try to get the tank
	ATank* ControlledTank = GetControlledTank();
	if (ControlledTank == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to find tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found tank: %s"), *(ControlledTank->GetName()));
	}
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
