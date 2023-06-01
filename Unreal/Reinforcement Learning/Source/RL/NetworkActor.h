// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking.h"
#include "Async/Async.h"
#include "Sockets.h"
#include "Kismet/GameplayStatics.h"
#include "PoleActor.h"
#include "NetworkActor.generated.h"

UCLASS()
class RL_API ANetworkActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	TArray<bool> hasInput;
	TArray<bool> appliedInput;
	TArray<int> input;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool isConnectionOpen = false;
	bool waitingForConnection = false;

	void OpenConnection();
	void CloseConnection();
	FSocket* ListenSocket = NULL;

	void ManageConnection();
	void ReceiveArrayMessage();
	FSocket* connectionSocket = NULL;
	TFuture<void> clientConnectionFinishedFuture;
	
	TArray<APoleActor*> poles;

	void SendObservation(int id);
};
