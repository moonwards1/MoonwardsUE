// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSubsystemMoonwards.h"

#include "OnlineIdentityMoonwards.h"
#include "OSMWCommon.h"


FOnlineSubsystemMoonwards::FOnlineSubsystemMoonwards(FName InInstanceName) : FOnlineSubsystemImpl(MOONWARDS_SUBSYSTEM, InInstanceName)
{
}

bool FOnlineSubsystemMoonwards::Init()
{
	MoonwardsIdentity = MakeShared<FOnlineIdentityMoonwards, ESPMode::ThreadSafe>();
	return true;
}

FText FOnlineSubsystemMoonwards::GetOnlineServiceName() const
{
	return NSLOCTEXT("OnlineSubsystemMoonwards", "OnlineServiceName", "OnlineSubsystemMoonwards");
}

FOnlineSubsystemMoonwards::~FOnlineSubsystemMoonwards()
{
    FOnlineSubsystemImpl::~FOnlineSubsystemImpl();
}

FName FOnlineSubsystemMoonwards::GetSubsystemName() const
{
	return FOnlineSubsystemImpl::GetSubsystemName();
}


IOnlineSessionPtr FOnlineSubsystemMoonwards::GetSessionInterface() const
{
	return nullptr;
}

IOnlineFriendsPtr FOnlineSubsystemMoonwards::GetFriendsInterface() const
{
	return nullptr;
}

IOnlineIdentityPtr FOnlineSubsystemMoonwards::GetIdentityInterface() const
{
	return MoonwardsIdentity;
}

IOnlineUserPtr FOnlineSubsystemMoonwards::GetUserInterface() const
{
	return FOnlineSubsystemImpl::GetUserInterface();
}

FString FOnlineSubsystemMoonwards::GetAppId() const
{
	return "";
}