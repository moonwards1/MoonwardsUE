// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSubsystemIdentity.h"

#include "Interfaces/OnlineIdentityInterface.h"

void UOnlineSubsystemIdentity::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
		OnlineIdentity->OnLoginCompleteDelegates->AddUObject(this, &UOnlineSubsystemIdentity::OnClientLoggedIn);
	}else
	{
		UE_LOG_ONLINE(Warning, TEXT("Online Subsystem Plugin: Failed to load Online Identity."));
	}
}

void UOnlineSubsystemIdentity::TryClientLogin(FString AuthToken) const
{
	FOnlineAccountCredentials AccountCredentials;
	AccountCredentials.Token = AuthToken;
	OnlineIdentity->Login(0, AccountCredentials);
}

FString UOnlineSubsystemIdentity::GetLocalClientDisplayName()
{
	return OnlineIdentity->GetPlayerNickname(0);
}

bool UOnlineSubsystemIdentity::IsLocalClientLoggedIn() const
{
	return OnlineIdentity->GetLoginStatus(0) == ELoginStatus::Type::LoggedIn;
}

ELoginStatusOss UOnlineSubsystemIdentity::GetLoginStatus(const FUniqueNetIdRepl& UniqueNetId) const
{
	return static_cast<ELoginStatusOss>(OnlineIdentity->GetLoginStatus(*UniqueNetId.GetUniqueNetId()));
}

bool UOnlineSubsystemIdentity::IsLoggedIn(const FUniqueNetIdRepl& UniqueNetId)
{
	if(UniqueNetId.IsValid() && UniqueNetId->IsValid())
	{
		return OnlineIdentity->GetLoginStatus(*UniqueNetId.GetUniqueNetId()) == ELoginStatus::Type::LoggedIn;
	}
	return false;
}

void UOnlineSubsystemIdentity::OnClientLoggedIn(int32 LocalUserNum, bool bWasSuccessful,const FUniqueNetId& UserId, const FString& Error) const
{
	// Handle this better
	if(!Error.IsEmpty() || !bWasSuccessful)
		UE_LOG_ONLINE(Error, TEXT("Failed to login "))
	
	if(/*is Client and*/ LocalUserNum == 0)
	{
		OnLocalClientLoginCompleted.Broadcast(bWasSuccessful);
	}
	//if(bIsServer)
	OnRemoteClientLoginCompleted.Broadcast(LocalUserNum, bWasSuccessful);
		
}
