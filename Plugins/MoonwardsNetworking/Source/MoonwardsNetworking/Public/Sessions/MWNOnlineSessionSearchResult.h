// #pragma once
// #ifndef MWNOnlineSessionSearchResult.h_H_
// #define MWNOnlineSessionSearchResult.h_H_
// 
// #endif

#pragma once

#include "CoreMinimal.h"

#include "OnlineSessionSettings.h"

#include "MWNOnlineSessionSearchResult.generated.h"

// Default advertise mode. I don't quite understand what "Ping" stands for here, but I assume some direct IP magic until proven otherwise.
// @TODO: Figure out what "Ping" stands for in advertisement context.
#define MWN_DEFAULT_ADVERTISE_MODE EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing

// A unique string such as we don't get all space-war game lobbies when dev testing
#define MWN_STEAM_DBG_STRING FName("SomeRandomString1337-55558")

#define MWN_SERVER_NAME FName("MWN_GAME_NAME")
#define MWN_LEVEL_NAME FName("MWN_LEVEL_NAME")

USTRUCT(BlueprintType)
struct FMWNOnlineSessionSearchResult
{
	GENERATED_BODY()

public:
 	FMWNOnlineSessionSearchResult() = default;
	FMWNOnlineSessionSearchResult(FOnlineSessionSearchResult const& result) : Result(result) {};
	FMWNOnlineSessionSearchResult(FOnlineSessionSearchResult&& result) : Result(result) {};

	FOnlineSessionSearchResult Result;
};