// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemImpl.h"

typedef TSharedPtr<class FOnlineIdentityMoonwards, ESPMode::ThreadSafe> FOnlineIdentityMoonwardsPtr;

/**
 * 
 */
class ONLINESUBSYSTEMMOONWARDS_API FOnlineSubsystemMoonwards :
	public FOnlineSubsystemImpl
{

	// FOnlineIdentityMW SessionInterface;

public:
	virtual bool Init() override;
	
	virtual FText GetOnlineServiceName() const override;
	virtual ~FOnlineSubsystemMoonwards() override;
	virtual FName GetSubsystemName() const override;

PACKAGE_SCOPE:
	/** Only the factory in OnlineSubsystemMWModule.cpp makes instances */
	FOnlineSubsystemMoonwards() = delete;
	explicit FOnlineSubsystemMoonwards(FName InInstanceName);
	virtual IOnlineSessionPtr GetSessionInterface() const override;
	virtual IOnlineFriendsPtr GetFriendsInterface() const override;
	virtual IOnlineIdentityPtr GetIdentityInterface() const override;
	virtual IOnlineUserPtr GetUserInterface() const override;
	virtual FString GetAppId() const override;
	
protected:
	FOnlineIdentityMoonwardsPtr MoonwardsIdentity;

};

typedef TSharedPtr<FOnlineSubsystemMoonwards, ESPMode::ThreadSafe> FOnlineSubsystemMoonwardsPtr;
