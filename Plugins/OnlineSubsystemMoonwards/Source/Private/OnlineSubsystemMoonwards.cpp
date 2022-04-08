// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineSubsystemMoonwards.h"

#include "OnlineIdentity/OnlineIdentityMoonwards.h"
#include "OSMWCommon.h"

FOnlineSubsystemMoonwards::FOnlineSubsystemMoonwards(FName InInstanceName) : FOnlineSubsystemImpl(MOONWARDS_SUBSYSTEM, InInstanceName)
{
	TickDelegate = FTickerDelegate::CreateRaw(this, &FTSTickerObjectBase::Tick);
	TickerDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate, 0);
}

bool FOnlineSubsystemMoonwards::Shutdown()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickerDelegateHandle);
	return FOnlineSubsystemImpl::Shutdown();
}

bool FOnlineSubsystemMoonwards::Init()
{
	MoonwardsIdentity = MakeShared<FOnlineIdentityMoonwards, ESPMode::ThreadSafe>();
	MoonwardsVoice = MakeShared<FOnlineVoiceMoonwards, ESPMode::ThreadSafe>(this);
	MoonwardsVoice->Init();

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
	return IOnlineSubsystem::Get(TEXT("NULL"))->GetSessionInterface();
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

IOnlineVoicePtr FOnlineSubsystemMoonwards::GetVoiceInterface() const
{
	return MoonwardsVoice;
}

FString FOnlineSubsystemMoonwards::GetAppId() const
{
	return "";
}

bool FOnlineSubsystemMoonwards::IsDedicated() const
{
	return bForceDedicated || IsRunningDedicatedServer();
}

bool FOnlineSubsystemMoonwards::IsLocalPlayer(const FUniqueNetId& UniqueId) const
{
	if (!IsDedicated())
	{
		IOnlineIdentityPtr IdentityInt = GetIdentityInterface();
		if (IdentityInt.IsValid())
		{
			for (int32 LocalUserNum = 0; LocalUserNum < MAX_LOCAL_PLAYERS; LocalUserNum++)
			{
				FUniqueNetIdPtr LocalUniqueId = IdentityInt->GetUniquePlayerId(LocalUserNum);
				if (LocalUniqueId.IsValid() && UniqueId == *LocalUniqueId)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool FOnlineSubsystemMoonwards::Tick(float DeltaTime)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_FOnlineSubsystemNull_Tick);

	if (!FOnlineSubsystemImpl::Tick(DeltaTime))
	{
		return false;
	}

	if (MoonwardsVoice.IsValid())// && bVoiceInterfaceInitialized)
	{
		MoonwardsVoice->Tick(DeltaTime);
	}

	return true;
}
