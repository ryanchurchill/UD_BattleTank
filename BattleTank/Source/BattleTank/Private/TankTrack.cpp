// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FixSlippage(DeltaTime);
	//FixFlying(DeltaTime);
}

void UTankTrack::FixSlippage(float DeltaTime)
{
	// Calculate the slippage speed
	// according to Ben: the component of speed in the tank right direction
	float SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// Work out the required acceleration this frame to correct
	FVector CorrectionAcceleration = (-SlippageSpeed / DeltaTime) * GetRightVector();

	// Calculate and apply sideways for (F=ma)
	FVector CorrectionForcePerTrack = (Tank->GetMass() * CorrectionAcceleration) / 2;
	Tank->AddForce(CorrectionForcePerTrack);
}

/*
EXPERIMENT! DOES NOT WORK AT ALL
*/
void UTankTrack::FixFlying(float DeltaTime)
{
	FString TankName = GetOwner()->GetName();
	FVector WorldUpVector = FVector(0, 0, 1);
	//auto UpVectorString = GetUpVector().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s: UpVector: %s"), *TankName, *GetUpVector().ToString())
	//UE_LOG(LogTemp, Warning, TEXT("%s: Component Velocity: %s"), *TankName, *GetComponentVelocity().ToString())

	// Component of speed in tank up direction?
	float UpSpeed = FVector::DotProduct(WorldUpVector, GetComponentVelocity());
	UE_LOG(LogTemp, Warning, TEXT("%s UpSpeed: %f"), *TankName, UpSpeed)

	FVector CorrectionAcceleration = (-UpSpeed / DeltaTime) * GetUpVector();
	FVector CorrectionForcePerTrack = (Tank->GetMass() * CorrectionAcceleration) / 2;
	Tank->AddForce(CorrectionForcePerTrack);
}

void UTankTrack::SetThrottle(float Throttle)
{
	auto ForceApplied = GetForwardVector() * TrackMaxDrivingForce * Throttle;
	auto ForceLocation = GetComponentLocation(); // the middle of the track
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}


void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit"))
}