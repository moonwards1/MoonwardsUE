#include "OnlineFactoryMoonwards.h"

FOnlineSubsystemMoonwardsPtr FOnlineFactoryMoonwards::OnlineSub = nullptr;
	
FOnlineFactoryMoonwards::FOnlineFactoryMoonwards() {}

FOnlineFactoryMoonwards::~FOnlineFactoryMoonwards() {}

IOnlineSubsystemPtr FOnlineFactoryMoonwards::CreateSubsystem(FName InstanceName)
{
	if(OnlineSub.IsValid())
	{
		UE_LOG_ONLINE(Warning, TEXT("Online Subsystem instance already exists, returning that instead."));
		return OnlineSub;
	}
		
	OnlineSub = MakeShared<FOnlineSubsystemMoonwards, ESPMode::ThreadSafe>(InstanceName);
	if (OnlineSub->IsEnabled())
	{
		UE_LOG_ONLINE(Log, TEXT("Moonwards API is being initialized."));

		if(!OnlineSub->Init())
		{
			UE_LOG_ONLINE(Warning, TEXT("Moonwards failed to initialize!"));
			OnlineSub->Shutdown();
			OnlineSub.Reset();
		}
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Moonwards disabled!"));
		OnlineSub->Shutdown();
		OnlineSub.Reset();
	}

	return OnlineSub;
}
