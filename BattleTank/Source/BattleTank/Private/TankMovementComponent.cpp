// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* _LeftTrack, UTankTrack* _RightTrack)
{
	if (!_LeftTrack || !_RightTrack) {
		UE_LOG(LogTemp, Error, TEXT("Could not initialize TankMovementComponent; Missing a track"));
		return;
	}
	LeftTrack = _LeftTrack;
	RightTrack = _RightTrack;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!LeftTrack || !RightTrack) {
		UE_LOG(LogTemp, Error, TEXT("Could not move: tracks not initialized"));
		return;
	}

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!LeftTrack || !RightTrack) {
		UE_LOG(LogTemp, Error, TEXT("Could not move: tracks not initialized"));
		return;
	}

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}