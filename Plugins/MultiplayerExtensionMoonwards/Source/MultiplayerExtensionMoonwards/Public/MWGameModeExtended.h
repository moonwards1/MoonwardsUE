// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "UObject/Object.h"
#include "MWGameModeExtended.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "GameMode Extended")
class MULTIPLAYEREXTENSIONMOONWARDS_API AMWGameModeExtended : public AGameModeBase
{
protected:
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
		const FString& Options, const FString& Portal) override;
public:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
		const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
private:
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	void InitLoggedInPlayer(const FUniqueNetId& UserId) const;
	void OnOnlineSubsystemLogin(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) const;
};
