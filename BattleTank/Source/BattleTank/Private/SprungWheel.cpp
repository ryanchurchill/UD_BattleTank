// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Tank.h"

// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
	SetRootComponent(MassWheelConstraint);

	Wheel = CreateDefaultSubobject<UStaticMeshComponent>(FName("Wheel"));
	Wheel->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
	SetupConstraint();
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASprungWheel::SetupConstraint()
{
	AActor* Body = GetAttachParentActor();
	if (ensure(Body))
	{
		UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(Body->GetRootComponent());
		if (ensure(BodyRoot))
		{
			MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Wheel, NAME_None);
		}
	}
}