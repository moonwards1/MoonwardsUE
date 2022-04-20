// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MWNPendingNetClient.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "MWNetworkingGameSubsystem.generated.h"

DECLARE_EVENT_OneParam(UMWNetworkingGameSubsystem, PendingClientFinishedEvent, bool)

/**
 * 
 */
UCLASS()
class MOONWARDSONLINEUTILS_API UMWNetworkingGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//@TODO: Cleanup, find a better way to host pending client members
	UPROPERTY()
	UMWNPendingNetClient* PendingNetClient;

	PendingClientFinishedEvent OnPendingClientFinished;

public:
	void ConnectToServer(UWorld* World, FURL& Url);

private:
	void PendingClientConnected(UMWNPendingNetClient* PendingNetClient, bool bSuccess) const;

};