// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "Projectile.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UE_LOG(LogTemp, Warning, TEXT("DONKEY: Tank %s constructed from Tank.cpp"), *GetName());
}

void ATank::AimAt(FVector HitLocation)
{
	if (TankAimingComponent == nullptr) {
		FString ThisTankName = GetName();
		UE_LOG(LogTemp, Error, TEXT("%s is missing TankAimingComponent"), *ThisTankName);
		return;
	}
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);	
}

void ATank::Fire()
{
	bool isReloaded = ((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds);
	if (Barrel && isReloaded) {
		UE_LOG(LogTemp, Warning, TEXT("Fire!"))

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

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("DONKEY: Tank %s BeginPlay from Tank.cpp"), *GetName());
	Super::BeginPlay();	
}

