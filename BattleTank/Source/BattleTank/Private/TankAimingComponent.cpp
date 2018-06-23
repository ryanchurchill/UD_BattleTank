// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; // TODO: should this really tick?

	// ...
}


// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::ElevateBarrelTowards(FVector AimDirection)
{
	// Fid difference between barrel rotation and AimDirection
	FRotator BarrelRotation = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotation;	
	Barrel->Elevate(1); // TODO: remove magic number
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	FString ThisTankName = GetOwner()->GetName();
	if (Barrel == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s failed to find Barrel"), *ThisTankName);
		return;
	}
	
	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// Calculate the OutLaunchVelocity
	if (UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.0f,
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	)) {		
		// Convert OutLaunchVelocity to unit vector
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();		
		UE_LOG(LogTemp, Warning, TEXT("%s aiming direction: %s"), *ThisTankName, *AimDirection.ToString());
		ElevateBarrelTowards(AimDirection);
	}	
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}
