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
	//FString TankName = GetOwner()->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("%s: Intend throw: %f"), *TankName, Throw);

	if (!LeftTrack || !RightTrack) {
		UE_LOG(LogTemp, Error, TEXT("Could not move: tracks not initialized"));
		return;
	}

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	//FString TankName = GetOwner()->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("%s: Intend turn throw: %f"), *TankName, Throw);

	if (!LeftTrack || !RightTrack) {
		UE_LOG(LogTemp, Error, TEXT("Could not move: tracks not initialized"));
		return;
	}

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call super, we are replacing

	FString TankName = GetOwner()->GetName();

	FVector CurrentTankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector ForwardIntention = MoveVelocity.GetSafeNormal();
	//UE_LOG(LogTemp, Warning, TEXT("%s: CurrentTankForward: %s"), *TankName, *CurrentTankForward.ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s: Forward intention: %s"), *TankName, *ForwardIntention.ToString());

	auto ForwardThrow = FVector::DotProduct(ForwardIntention, CurrentTankForward);
	IntendMoveForward(ForwardThrow);

	auto RightThrowVector = FVector::CrossProduct(CurrentTankForward, ForwardIntention);
	//UE_LOG(LogTemp, Warning, TEXT("Intend turn throw: %s"), *RightThrowVector.ToString());
	IntendTurnRight(RightThrowVector.Z);
}
