// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAiController.h"
#include "TankAimingComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Tank.h"

// Depends on movement component via pathfinding system

void ATankAiController::BeginPlay()
{
	Super::BeginPlay();	

	// try to get the tank
	APawn* ControlledTank = GetControlledTank();
	if (!ensure(ControlledTank)) {
		return;
	}

	AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
}

void ATankAiController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		ATank* PosessedTank = Cast<ATank>(InPawn);
		if (!ensure(PosessedTank)) { return; }

		// Subscribe local method to tank's death event
		PosessedTank->OnDeath.AddUniqueDynamic(this, &ATankAiController::OnPosessedTankDeath);
	}
}

void ATankAiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APawn* PlayerTank = GetPlayerTank();
	if (ensure(PlayerTank)) {
		APawn* ControlledTank = GetControlledTank();

		// move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim towards player tank
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		// fire at player when locked
		if (AimingComponent->GetFiringState() == EFiringState::Locked)
		{
			AimingComponent->Fire();
		}
	}	
}

APawn* ATankAiController::GetControlledTank() const
{
	return GetPawn();
}

APawn * ATankAiController::GetPlayerTank() const
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerPawn)) {
		return nullptr;
	}

	return PlayerPawn;	
}

void ATankAiController::OnPosessedTankDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Tank Died!"));
}