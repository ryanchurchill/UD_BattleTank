// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel* _Barrel, UTankTurret* _Turret)
{
	Barrel = _Barrel;
	Turret = _Turret;
}


// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	FString ThisTankName = GetOwner()->GetName();
	if (!ensure(Barrel)) {
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
		ElevateBarrelTowards(AimDirection);
		RotateTurretTowards(AimDirection);
	}
}

void UTankAimingComponent::ElevateBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel)) { return; }

	// Find difference between barrel rotation and AimDirection
	FRotator BarrelRotation = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotation;
	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::RotateTurretTowards(FVector AimDirection)
{
	if (!ensure(Turret)) { return; }

	// Find difference between turret rotation and AimDirection
	FRotator TurretRotation = Turret->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - TurretRotation;
	Turret->Rotate(DeltaRotator.Yaw);
}
