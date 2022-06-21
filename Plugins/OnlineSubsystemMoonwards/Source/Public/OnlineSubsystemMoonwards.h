// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemImpl.h"

#include "OnlineIdentity/OnlineIdentityMoonwards.h"

#include "OnlineVoice/OnlineVoiceMoonwards.h"


/**
 * 
 */
class ONLINESUBSYSTEMMOONWARDS_API FOnlineSubsystemMoonwards :
	public FOnlineSubsystemImpl
{

private:
	bool bTickerStarted = true;
	bool bForceDedicated = false;
	FTickerDelegate TickDelegate;
	FTSTicker::FDelegateHandle TickerDelegateHandle;
public:
	virtual bool Shutdown() override;
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
	virtual IOnlineVoicePtr GetVoiceInterface() const override;
	virtual FString GetAppId() const override;
	virtual bool IsDedicated() const override;
	virtual bool IsLocalPlayer(const FUniqueNetId& UniqueId) const override;
	virtual bool Tick(float DeltaTime) override;

protected:
	FOnlineIdentityMoonwardsPtr MoonwardsIdentity;
	FOnlineVoiceMoonwardsPtr MoonwardsVoice;
};

typedef TSharedPtr<FOnlineSubsystemMoonwards, ESPMode::ThreadSafe> FOnlineSubsystemMoonwardsPtr;
