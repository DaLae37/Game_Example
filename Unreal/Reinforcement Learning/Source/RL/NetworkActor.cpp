// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkActor.h"

// Sets default values
ANetworkActor::ANetworkActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANetworkActor::BeginPlay()
{
	Super::BeginPlay();
	OpenConnection();
	
	TSubclassOf<APoleActor> classToFind;
	TArray<AActor*> find;
	classToFind = APoleActor::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, find);
	UE_LOG(LogTemp, Warning, TEXT("Get Actor %d"), find.Num());

	APoleActor* caster = NULL;

	for(int i=0; i<find.Num(); i++){
		caster = Cast<APoleActor>(find[i]);
		poles.Add(caster);
	}
	hasInput.Init(false, poles.Num());
	appliedInput.Init(false, poles.Num());
	input.Init(0, poles.Num());
}

void ANetworkActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);
	CloseConnection();
}

// Called every frame
void ANetworkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (int id = 0; id < poles.Num(); id++) {
		ManageConnection();

		if (input[id] == 10) {
			poles[id]->ResetEnvironment(id);
		}
		else if (input[id] == 11) {

		}
		else {
			poles[id]->ApplySpeed(DeltaTime);
		}

		if (input[id] != 10 && appliedInput[id]) {
			appliedInput[id] = false;
			SendObservation(id);
		}

		if (input[id] == 10) {
			input[id] = 11;
		}
		if (hasInput[id]) {
			hasInput[id] = false;
		}
	}
}

void ANetworkActor::OpenConnection()
{
	if (!isConnectionOpen) {
		UE_LOG(LogTemp, Warning, TEXT("Open Connection"));
		isConnectionOpen = true;
		waitingForConnection = true;

		FIPv4Address IPAddress;
		FIPv4Address::Parse(FString("127.0.0.1"), IPAddress);
		FIPv4Endpoint endPoint(IPAddress, (uint16)5050);

		ListenSocket = FTcpSocketBuilder(TEXT("TcpSocket")).AsReusable();

		ISocketSubsystem* socketSubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		ListenSocket->Bind(*socketSubSystem->CreateInternetAddr(endPoint.Address.Value, endPoint.Port));
		ListenSocket->Listen(1);
		UE_LOG(LogTemp, Warning, TEXT("Listening"));
	}
}

void ANetworkActor::CloseConnection()
{
	if (isConnectionOpen) {
		UE_LOG(LogTemp, Warning, TEXT("Close Connection"));
		isConnectionOpen = false;
		ListenSocket->Close();
	}
}

void ANetworkActor::ManageConnection()
{
	if (waitingForConnection) {
		TSharedRef<FInternetAddr> remoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		bool hasConnection = false;
		if (ListenSocket->HasPendingConnection(hasConnection) && hasConnection) {
			connectionSocket = ListenSocket->Accept(*remoteAddress, TEXT("Connection"));
			waitingForConnection = false;
			UE_LOG(LogTemp, Warning, TEXT("Incoming Connection"));

			clientConnectionFinishedFuture = Async(EAsyncExecution::LargeThreadPool, [&]() {
				UE_LOG(LogTemp, Warning, TEXT("Receive Thread"));
				ReceiveArrayMessage();
				}
			);
		}
	}
}

void ANetworkActor::ReceiveArrayMessage()
{
	while (isConnectionOpen) {
		uint32 size;
		TArray<uint8> receivedData;

		if (connectionSocket->HasPendingData(size)) {
			receivedData.Init(0, 1024);
			int32 read = 0;
			connectionSocket->Recv(receivedData.GetData(), receivedData.Num(), read);

			if (receivedData.Num() > 0) {
				TArray<float> receivedArray;
				const int32 numElements = receivedData.Num() / sizeof(float);
				receivedArray.SetNum(numElements);
				FMemory::Memcpy(receivedArray.GetData(), receivedData.GetData(), receivedArray.Num());

				int id = int(receivedArray[0]);
				input[id] = int(receivedArray[1]);

				UE_LOG(LogTemp, Warning, TEXT("ID : %d, Value : %d"), id, input[id]);
				hasInput[id] = true;

				if (hasInput[id]) {
					appliedInput[id] = true;
					if (input[id] == 10) {

					}
					else {
						poles[id]->MoveXAxis(input[id]);
					}
				}
			}
		}
	}
}

void ANetworkActor::SendObservation(int id)
{
	FVector velocity2 = poles[id]->Pole->GetPhysicsAngularVelocityInDegrees();
	FVector velocity = poles[id]->Base->GetPhysicsLinearVelocity();
	FRotator rotator = poles[id]->Pole->GetRelativeRotation();
	FVector currentLocaton = poles[id]->Base->GetComponentLocation();

	TArray<float> state = { float(rotator.Pitch), float(currentLocaton.X), float(velocity.X), float(velocity2.Y) };

	uint32 arraySize = state.Num();
	int32 sent = 0;
	connectionSocket->Send(reinterpret_cast<const uint8*>(&arraySize), sizeof(uint32), sent);

	TArray<uint8> arrayData;
	arrayData.SetNumUninitialized(arraySize * sizeof(float));
	FMemory::Memcpy(arrayData.GetData(), state.GetData(), arrayData.Num());
	sent = 0;
	connectionSocket->Send(arrayData.GetData(), arrayData.Num(), sent);
}
