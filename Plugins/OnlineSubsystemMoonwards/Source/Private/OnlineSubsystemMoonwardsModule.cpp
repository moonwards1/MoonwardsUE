// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemMoonwardsModule.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemMoonwards.h"
#include "OSMWCommon.h"

IMPLEMENT_MODULE(FOnlineSubsystemMoonwardsModule, OnlineSubsystemMoonwards);

void FOnlineSubsystemMoonwardsModule::StartupModule()
{
	UE_LOG_ONLINE(Log, TEXT("Moonwards Online Subsystem Module Startup!"));

	MoonwardsFactory = new FOnlineFactoryMoonwards();

	// Create and register our singleton factory with the main online subsystem for easy access
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService(MOONWARDS_SUBSYSTEM, MoonwardsFactory);
}

void FOnlineSubsystemMoonwardsModule::ShutdownModule()
{
	if(MoonwardsFactory)
	{
		UE_LOG_ONLINE(Log, TEXT("Moonwards Online Subsystem Shutdown!"));

		FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
		OSS.UnregisterPlatformService(MOONWARDS_SUBSYSTEM);

		delete MoonwardsFactory;
		MoonwardsFactory = nullptr;
	}
}
