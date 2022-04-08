// Fill out your copyright notice in the Description page of Project Settings.


#include "MWNFunctionLibrary.h"

#include "MWLogger.h"
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
		MW_LOG("Server listening started.", EMWLogCategory::Online)
	}
	else
	{
		MW_LOG("Failed to start listen server.", EMWLogCategory::Online)
	}
	return bSuccess;
}

void UMWNFunctionLibrary::ConnectToServerSeamlessly(const UObject* WorldContextObject)
{
	FURL Url;
	Url.Host = "127.0.0.1";
	Url.Port = 7777;
	
	const auto PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	PlayerController->ClientTravel("127.0.0.1", ETravelType::TRAVEL_Absolute, false);
	
	// UWorld* World = PlayerController->GetWorld();
	// const UGameInstance* GameInstance = World->GetGameInstance();
	// UMWNetworkingGameSubsystem* NetworkingSubsystem = GameInstance->GetSubsystem<UMWNetworkingGameSubsystem>();  
	// NetworkingSubsystem->ConnectToServer(World, Url);
}
