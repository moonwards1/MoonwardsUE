// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"

#include "Interfaces/OnlineIdentityInterface.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystemIdentity.generated.h"

// Called for all existing clients, used by the server.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRemoteClientLoginCompletedDelegate, int32, LocalUserNum, bool, bWasSuccessful);

// Called for clients attempting to connect to the server.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocalClientLoginCompletedDelegate, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class OSSBLUEPRINTLIBRARY_API UOnlineSubsystemIdentity : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	IOnlineIdentityPtr OnlineIdentity;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/*
	 * CLIENT 
	 */

	// Called when the local client's login request to the server has received a response.
	UPROPERTY(BlueprintAssignable, Category = "OSS Identity | Client")
	FLocalClientLoginCompletedDelegate OnLocalClientLoginCompleted;
	

	UFUNCTION(BlueprintCallable, Category = "OSS Identity | Client")
	void TryClientLogin(FString& AuthToken) const;

	// UFUNCTION(BlueprintCallable, Category = "OSS Identity | Client")
	// FString GetLocalClientUniqueId();
	
	UFUNCTION(BlueprintCallable, Category = "OSS Identity | Client")
	FString GetLocalClientDisplayName();
	/*
	 * SERVER 
	 */
	
	// Called when a new remote client has logged into this server.
	UPROPERTY(BlueprintAssignable, Category = "OSS Identity | Server")
	FRemoteClientLoginCompletedDelegate OnRemoteClientLoginCompleted;

	// Called after a client's login request has received a response.
	void OnClientLoggedIn(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) const;

};
