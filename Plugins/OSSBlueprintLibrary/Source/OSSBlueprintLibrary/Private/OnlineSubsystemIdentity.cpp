// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSubsystemIdentity.h"

void UOnlineSubsystemIdentity::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	OnlineIdentity = IOnlineSubsystem::Get()->GetIdentityInterface();
	OnlineIdentity->OnLoginCompleteDelegates->AddUObject(this, &UOnlineSubsystemIdentity::OnClientLoggedIn);
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

ELoginStatusOss UOnlineSubsystemIdentity::GetLoginStatus(int32 LocalUserNum) const
{
	return static_cast<ELoginStatusOss>(OnlineIdentity->GetLoginStatus(LocalUserNum));
}

void UOnlineSubsystemIdentity::OnClientLoggedIn(int32 LocalUserNum, bool bWasSuccessful,const FUniqueNetId& UserId, const FString& Error) const
{
	// Handle this better
	if(!Error.IsEmpty() || !bWasSuccessful)
		UE_LOG_ONLINE(Error, TEXT("Failed to login "))
	
	if(/*is Client and*/ LocalUserNum == 0)
		
		OnLocalClientLoginCompleted.Broadcast(bWasSuccessful);
	//if(bIsServer)
		OnRemoteClientLoginCompleted.Broadcast(LocalUserNum, bWasSuccessful);
		
}
