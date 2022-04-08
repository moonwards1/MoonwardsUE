// Fill out your copyright notice in the Description page of Project Settings.


#include "MWGameModeExtended.h"

#include "OnlineSubsystem.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/VoiceInterface.h"

void AMWGameModeExtended::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	APlayerState const* PlayerState = NewPlayer->PlayerState;
	FUniqueNetIdPtr const UniqueNetId = PlayerState->GetUniqueId().GetUniqueNetId();

	if(UniqueNetId.IsValid())
		InitLoggedInPlayer(*UniqueNetId);
}

void AMWGameModeExtended::BeginPlay()
{
	Super::BeginPlay();
	if(IOnlineSubsystem const* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		// Temporary until we refresh after logging in.
		if(IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->OnLoginCompleteDelegates->AddUObject(this, &AMWGameModeExtended::OnOnlineSubsystemLogin);
		}
	}
}

void AMWGameModeExtended::InitLoggedInPlayer(const FUniqueNetId& UserId)
{
	if(IOnlineSubsystem const* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if(UserId.IsValid())
		if(IOnlineVoicePtr const OnlineVoice = OnlineSubsystem->GetVoiceInterface())
		{
			if(OnlineSubsystem->IsLocalPlayer(UserId))
				OnlineVoice->RegisterLocalTalker(0);
			else
			{
				OnlineVoice->RegisterRemoteTalker(UserId);
			}
		}
	}
}

void AMWGameModeExtended::OnOnlineSubsystemLogin(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	if(bWasSuccessful && UserId.IsValid())
		InitLoggedInPlayer(UserId);
}

