// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "OnlineFactoryMoonwards.h"

#include "Modules/ModuleManager.h"

class FOnlineSubsystemMoonwardsModule : public IModuleInterface
{
private:
	/** Class responsible for creating instance(s) of the subsystem */
	FOnlineFactoryMoonwards* MoonwardsFactory;
	
public:
	explicit FOnlineSubsystemMoonwardsModule()
		: MoonwardsFactory(nullptr)
	{
	}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

	virtual bool SupportsAutomaticShutdown() override
	{
		return false;
	}
};
