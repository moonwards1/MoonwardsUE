#include "OnlineFactoryMoonwards.h"

FOnlineFactoryMoonwards::FOnlineFactoryMoonwards() {}

FOnlineFactoryMoonwards::~FOnlineFactoryMoonwards() {}

IOnlineSubsystemPtr FOnlineFactoryMoonwards::CreateSubsystem(FName InstanceName)
{
	FOnlineSubsystemMoonwardsPtr OnlineSub = MakeShared<FOnlineSubsystemMoonwards, ESPMode::ThreadSafe>(InstanceName);
	if (OnlineSub->IsEnabled())
	{
		UE_LOG_ONLINE(Log, TEXT("Moonwards API is being initialized."));

		if(!OnlineSub->Init())
		{
			UE_LOG_ONLINE(Error, TEXT("Moonwards failed to initialize!"));
			OnlineSub->Shutdown();
			OnlineSub.Reset();
		}
	}
	else
	{
		UE_LOG_ONLINE(Error, TEXT("Moonwards Subsystem is disabled!"));
		OnlineSub->Shutdown();
		OnlineSub.Reset();
	}

	return OnlineSub;
}
