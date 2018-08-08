// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* _LeftTrack, UTankTrack* _RightTrack)
{
	if (!ensure(_LeftTrack && _RightTrack)) { return; }
	LeftTrack = _LeftTrack;
	RightTrack = _RightTrack;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	FString TankName = GetOwner()->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("%s: Intend throw: %f"), *TankName, Throw);

	if (!ensure(LeftTrack && RightTrack)) { return; }

	//UE_LOG(LogTemp, Warning, TEXT("2 %s: Intend throw: %f"), *TankName, Throw);

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	//FString TankName = GetOwner()->GetName();
	//UE_LOG(LogTemp, Warning, TEXT("%s: Intend turn throw: %f"), *TankName, Throw);

	if (!ensure(LeftTrack && RightTrack)) { return; }

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call super, we are replacing

	FString TankName = GetOwner()->GetName();

	FVector CurrentTankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector ForwardIntention = MoveVelocity.GetSafeNormal();
	//UE_LOG(LogTemp, Warning, TEXT("%s: Forward intention: %s"), *TankName, *ForwardIntention.ToString());

	auto ForwardThrow = FVector::DotProduct(ForwardIntention, CurrentTankForward);
	IntendMoveForward(ForwardThrow);

	auto RightThrowVector = FVector::CrossProduct(CurrentTankForward, ForwardIntention);
	//UE_LOG(LogTemp, Warning, TEXT("Intend turn throw: %s"), *RightThrowVector.ToString());
	IntendTurnRight(RightThrowVector.Z);
}
