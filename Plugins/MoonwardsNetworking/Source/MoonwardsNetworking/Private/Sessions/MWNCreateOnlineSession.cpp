// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/MWNCreateOnlineSession.h"
#include "Sessions/MWNOnlineSessionSearchResult.h"
#include "MWLogger.h"

UMWNCreateOnlineSession::UMWNCreateOnlineSession(const FObjectInitializer& ObjectInitializer) 
	: UBlueprintAsyncActionBase(ObjectInitializer)
{
}

UMWNCreateOnlineSession* UMWNCreateOnlineSession::CreateSession(const UObject* WorldContextObject, const FString ServerName,  const bool bDestroyExistingSession)
{

	UMWNCreateOnlineSession* BlueprintNode = NewObject<UMWNCreateOnlineSession>();
	BlueprintNode->WorldContextObject = WorldContextObject;
	BlueprintNode->ServerName = ServerName;
	BlueprintNode->OnlineSubsystem = IOnlineSubsystem::Get();
	BlueprintNode->bDestroyExistingSession = bDestroyExistingSession;
	return BlueprintNode;
}

void UMWNCreateOnlineSession::Activate()
{
	if(bIsRunning)
	{
		Failed.Broadcast();
		return;
	}
	
	bIsRunning = true;
	
	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface)
	{
		MW_LOG(FString("Could not retrieve session interface."), EMWLogCategory::Online);
		return;
	}

	const auto CurrentSession = SessionInterface->GetNamedSession(GameSessionName);
	if(!CurrentSession) // Create a new session when none exists
	{
		CreateSession();
	}else if(bDestroyExistingSession) // Destroy and re-create the session
	{
		RecreateSession();
	}else // Fail
	{
		MW_LOG("Failed to create online session, a session already exists.");
		Failed.Broadcast();
	}
	
}

void UMWNCreateOnlineSession::CreateSession()
{
	MW_LOG("Creating online session...", EMWLogCategory::Online);

	// We could create some of these from UI/configs at some point.
	FOnlineSessionSettings SessionSettings{};
	SessionSettings.BuildUniqueId = 1337;
	SessionSettings.NumPublicConnections = 300;
	SessionSettings.NumPrivateConnections = 300;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bIsLANMatch = bPlayOffline;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(SETTING_MAPNAME, FString("Moonwards"), MWN_DEFAULT_ADVERTISE_MODE);
	SessionSettings.Set(MWN_STEAM_DBG_STRING, true, MWN_DEFAULT_ADVERTISE_MODE);

	SessionSettings.Set(MWN_SERVER_NAME, this->ServerName, MWN_DEFAULT_ADVERTISE_MODE);

	OnCreateSessionCompleteDelegateHandle = SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMWNCreateOnlineSession::CreateSessionCompleted);
	SessionInterface->CreateSession(0, GameSessionName, SessionSettings);
}

void UMWNCreateOnlineSession::RecreateSession()
{
	OnDestroySessionCompleteDelegateHandle = SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMWNCreateOnlineSession::DestroySessionCompleted);
	SessionInterface->DestroySession(GameSessionName);
}

void UMWNCreateOnlineSession::CreateSessionCompleted(FName SessionName, const bool bSucceeded) const
{
	SessionInterface->OnCreateSessionCompleteDelegates.Remove(OnCreateSessionCompleteDelegateHandle);
	if(bSucceeded)
	{
		MW_LOG("Successfully created online session.", EMWLogCategory::Online);
		Succeeded.Broadcast();
	}else
	{
		MW_LOG("Failed to create new online session.", EMWLogCategory::Online);
		Failed.Broadcast();
	}
}

void UMWNCreateOnlineSession::DestroySessionCompleted(FName SessionName, const bool bSucceeded)
{
	SessionInterface->OnDestroySessionCompleteDelegates.Remove(OnDestroySessionCompleteDelegateHandle);
	if(bSucceeded)
	{
		CreateSession();
	}else
	{
		MW_LOG("Failed to destroy existing session.", EMWLogCategory::Online);
		Failed.Broadcast();
	}
}