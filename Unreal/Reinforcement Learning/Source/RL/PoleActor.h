// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoleActor.generated.h"

UCLASS()
class RL_API APoleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoleActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* Base;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* Pole;

	UPROPERTY(EditAnywhere)
		float MaxSpeed;
	UPROPERTY(EditAnywhere)
		float MotorPower;
	UPROPERTY(EditAnyWhere)
		float CurrentMotorSpeed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetEnvironment(int id);
	void MoveXAxis(int AxisValue);
	void ApplySpeed(float deltaTime);
};
