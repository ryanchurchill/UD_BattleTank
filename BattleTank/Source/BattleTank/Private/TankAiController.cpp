// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAiController.h"


void ATankAiController::BeginPlay()
{
	Super::BeginPlay();	

	// try to get the tank
	ATank* ControlledTank = GetControlledTank();
	if (ControlledTank == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("AiController Failed to find tank"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AiController Found tank: %s"), *(ControlledTank->GetName()));
	}
}

ATank* ATankAiController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

