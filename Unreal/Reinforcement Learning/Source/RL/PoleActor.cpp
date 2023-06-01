// Fill out your copyright notice in the Description page of Project Settings.


#include "PoleActor.h"

// Sets default values
APoleActor::APoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BASE"));
	Pole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("POLE"));
}

// Called when the game starts or when spawned
void APoleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APoleActor::ResetEnvironment(int id)
{
	FVector currentLocation = Base->GetComponentLocation();
	currentLocation.X = 0.0f;
	currentLocation.Y = 300.0f * id;
	currentLocation.Z = 0.0f;

	Base->SetWorldLocation(currentLocation);

	CurrentMotorSpeed = 0;

	FRotator rotator = Pole->GetRelativeRotation();
	rotator.Pitch = 0.0f;
	rotator.Roll = 0.0f;
	rotator.Yaw = 0.0f;
	Pole->SetRelativeRotation(rotator);

	currentLocation = Pole->GetComponentLocation();
	currentLocation.X = 0.0f;
	currentLocation.Y = 300.0f * id;
	currentLocation.Z = 10.0f;
	Pole->SetWorldLocation(currentLocation);

	FVector angularVelocity = FVector();
	Pole->SetPhysicsAngularVelocityInDegrees(angularVelocity);
	Pole->SetAllPhysicsLinearVelocity(angularVelocity);
}

void APoleActor::MoveXAxis(int AxisValue)
{
	if (AxisValue == 0) {
		AxisValue = -1;
	}
	UE_LOG(LogTemp, Warning, TEXT("Move %d"), AxisValue);
	CurrentMotorSpeed += AxisValue * MotorPower;
	CurrentMotorSpeed = FMath::Clamp(CurrentMotorSpeed, -MaxSpeed, MaxSpeed);
}

void APoleActor::ApplySpeed(float deltaTime)
{
	FVector currentLocation = Base->GetComponentLocation();
	currentLocation.X = currentLocation.X + (CurrentMotorSpeed * deltaTime);
	Base->SetWorldLocation(currentLocation);
	CurrentMotorSpeed *= 0.9f;
}