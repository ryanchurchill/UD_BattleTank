// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "Classes/Engine/World.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	float ElevationChange = FMath::Clamp<float>(RelativeSpeed, -1, 1) * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float NewElevation = FMath::Clamp(RelativeRotation.Pitch + ElevationChange, MinElevationDeg, MaxElevationDeg);	
	SetRelativeRotation(FRotator(NewElevation, 0, 0));
}

