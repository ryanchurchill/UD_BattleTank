// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::FixSlippage()
{
	// Calculate the slippage speed
	// according to Ben: the component of speed in the tank right direction
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// Work out the required acceleration this frame to correct
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector CorrectionAcceleration = (-SlippageSpeed / DeltaTime) * GetRightVector();

	// Calculate and apply sideways for (F=ma)
	FVector CorrectionForcePerTrack = (Tank->GetMass() * CorrectionAcceleration) / 2;
	Tank->AddForce(CorrectionForcePerTrack);
}

void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
	//CurrentThrottle = Throttle;
	
}

void UTankTrack::DriveTrack()
{
	auto ForceApplied = GetForwardVector() * TrackMaxDrivingForce * CurrentThrottle;
	auto ForceLocation = GetComponentLocation(); // the middle of the track
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	FixSlippage();
	CurrentThrottle = 0;
}