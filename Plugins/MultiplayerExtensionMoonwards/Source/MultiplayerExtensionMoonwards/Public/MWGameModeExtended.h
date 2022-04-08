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
	GENERATED_BODY()
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	void InitLoggedInPlayer(const FUniqueNetId& UserId);
	void OnOnlineSubsystemLogin(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
};
