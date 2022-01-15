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
	 */
	UFUNCTION(BlueprintCallable, Category = "Moonwards Networking | Server", meta = (WorldContext = "WorldContextObject"))
	static void ConnectToServerSeamlessly(const UObject* WorldContextObject);
};
