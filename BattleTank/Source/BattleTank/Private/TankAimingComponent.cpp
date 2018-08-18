// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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

	// force an initial reload
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// set firing status
	if (IsReloading()) {
		FiringStatus = EFiringState::Reloading;
	}
	else if (IsBarrelMoving()) {
		FiringStatus = EFiringState::Aiming;
	}
	else {
		FiringStatus = EFiringState::Locked;
	}
}


void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }
	
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
		LastAimDirection = AimDirection;
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

	auto Yaw = DeltaRotator.Yaw;
	if (Yaw > 180) {
		Yaw -= 360;
	}
	Turret->Rotate(Yaw);
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) { return; }

	if (FiringStatus != EFiringState::Reloading) {
		// Spawn a projectile at the socket location of the barrel
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

bool UTankAimingComponent::IsReloading()
{
	return ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds);
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	FVector CurrentBarrelVector = Barrel->GetForwardVector();
	return !CurrentBarrelVector.Equals(LastAimDirection, 0.01);
}