// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAiController.generated.h"

class UTankAimingComponent;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAiController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;	
	
	
private:
	virtual void Tick(float DeltaTime) override;
	APawn* GetControlledTank() const;
	APawn* GetPlayerTank() const;

	// How close should AI tank get to player 
	float AcceptanceRadius = 100000; // TODO

	UTankAimingComponent* AimingComponent = nullptr;
};
