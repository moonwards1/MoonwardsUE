// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/MWNCreateOnlineSession.h"
#include "Sessions/MWNOnlineSessionSearchResult.h"

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
		UE_LOG(LogNet, Log, TEXT("Could not retrieve session interface."));
		return;
	}

	const auto CurrentSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if(!CurrentSession) // Create a new session when none exists
	{
		CreateSession();
	}else if(bDestroyExistingSession) // Destroy and re-create the session
	{
		RecreateSession();
	}else // Fail
	{
		UE_LOG(LogNet, Log, TEXT("Failed to create online session, a session already exists."));
		Failed.Broadcast();
	}
	
}

void UMWNCreateOnlineSession::CreateSession()
{
	UE_LOG(LogNet, Log, TEXT("Creating online session..."));

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
	SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
}

void UMWNCreateOnlineSession::RecreateSession()
{
	OnDestroySessionCompleteDelegateHandle = SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMWNCreateOnlineSession::DestroySessionCompleted);
	SessionInterface->DestroySession(NAME_GameSession);
}

void UMWNCreateOnlineSession::CreateSessionCompleted(FName SessionName, const bool bSucceeded) const
{
	SessionInterface->OnCreateSessionCompleteDelegates.Remove(OnCreateSessionCompleteDelegateHandle);
	if(bSucceeded)
	{
		UE_LOG(LogNet, Log, TEXT("Successfully created online session."));
		Succeeded.Broadcast();
	}else
	{
		UE_LOG(LogNet, Log, TEXT("Failed to create new online session."));
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
		UE_LOG(LogNet, Log, TEXT("Failed to destroy existing session."));
		Failed.Broadcast();
	}
}