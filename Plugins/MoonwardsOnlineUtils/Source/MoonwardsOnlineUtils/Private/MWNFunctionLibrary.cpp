// Fill out your copyright notice in the Description page of Project Settings.


#include "MWNFunctionLibrary.h"

#include "CoreMinimal.h"
#include "MWNetworkingGameSubsystem.h"

#include "Engine/PendingNetGame.h"

#include "Kismet/GameplayStatics.h"

#include "Net/NetworkProfiler.h"


bool UMWNFunctionLibrary::StartListenServer(const UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject->GetWorld();
	FURL Url;
	Url.Host = FString("0.0.0.0");
	Url.Port = 7777;
	const bool bSuccess = World->Listen(Url);
	if(bSuccess)
	{
		UE_LOG(LogNet, Log, TEXT("Server listening started."));
	}
	else
	{
		UE_LOG(LogNet, Log, TEXT("Failed to start listen server."));
	}
	return bSuccess;
}

void UMWNFunctionLibrary::ConnectToServerSeamlessly(const UObject* WorldContextObject, FString IpAddress, int Port)
{
	FURL Url;
	Url.Host = IpAddress;
	Url.Port = Port;
	
	const auto PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	
	UWorld* World = PlayerController->GetWorld();
	const UGameInstance* GameInstance = World->GetGameInstance();
	UMWNetworkingGameSubsystem* NetworkingSubsystem = GameInstance->GetSubsystem<UMWNetworkingGameSubsystem>();  
	NetworkingSubsystem->ConnectToServer(World, Url);
}
