// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MWNFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MOONWARDSNETWORKING_API UMWNFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Moonwards Networking | Server", meta = (WorldContext = "WorldContextObject"))
	static bool StartListenServer(const UObject* WorldContextObject);

	
	/**
	 * Connects to the server without reloading the map.
	 * @param WorldContextObject
	 * @param IpAddress
	 * @param Port 
	 */
	UFUNCTION(BlueprintCallable, Category = "Moonwards Networking | Server", meta = (WorldContext = "WorldContextObject"))
	static void ConnectToServerSeamlessly(const UObject* WorldContextObject, FString IpAddress = "127.0.0.1", int Port = 7777);
};
