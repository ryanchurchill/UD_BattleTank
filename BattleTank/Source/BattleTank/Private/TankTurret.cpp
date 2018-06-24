// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Classes/Engine/World.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	float RotationChange = FMath::Clamp<float>(RelativeSpeed, -1, 1) * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	float NewRotation = RelativeRotation.Yaw + RotationChange;
	SetRelativeRotation(FRotator(0, NewRotation, 0));
}


