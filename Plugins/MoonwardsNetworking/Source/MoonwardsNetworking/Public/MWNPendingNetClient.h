// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CoreMinimal.h"
#include "Net/Core/Connection/NetCloseResult.h"
#include "MWNPendingNetClient.generated.h"

//@TODO: Update this to UE5 standard.
DECLARE_EVENT_TwoParams(UMWNPendingNetClient, ConnectionSequenceCompletedEvent, UMWNPendingNetClient*, bool);
/**
 * A wrapper that helps with client -> server connection and then notifies when ready to join, after a successful connection & handshake.
 */
UCLASS()
class MOONWARDSNETWORKING_API UMWNPendingNetClient : public UObject, public FNetworkNotify, public FTickableGameObject
{
public:

private:
	GENERATED_BODY()

public:
	
	UPROPERTY()
	UNetDriver*		NetDriver;

	UPROPERTY()
	UWorld* World;
	
	FURL					URL;
	bool					bSuccessfullyConnected;
	FString					ConnectionError;

	ConnectionSequenceCompletedEvent OnConnectionSequenceCompleted;

	virtual void TryConnect(UWorld* World, FURL& Url);
	
	virtual void NotifyControlMessage(UNetConnection* Connection, uint8 MessageType, FInBunch& Bunch) override;
	virtual EAcceptConnection::Type NotifyAcceptingConnection() override;
	virtual void NotifyAcceptedConnection(UNetConnection* Connection) override;
	virtual bool NotifyAcceptingChannel(UChannel* Channel) override;

	virtual void FinalizeEncryptedConnection(const FEncryptionKeyResponse& EncryptionKeyResponse, TWeakObjectPtr<UNetConnection> NetConnection);
	virtual void RequestJoin() const;

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
private:
	void InitiateConnectSequence() const;
	bool bIsTickable = false;

};
