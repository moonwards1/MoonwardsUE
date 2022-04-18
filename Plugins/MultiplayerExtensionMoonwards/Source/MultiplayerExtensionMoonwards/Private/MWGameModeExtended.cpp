// Fill out your copyright notice in the Description page of Project Settings.


#include "MWGameModeExtended.h"

#include "OnlineSubsystem.h"

#include "GameFramework/GameSession.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/VoiceInterface.h"

#include "Kismet/GameplayStatics.h"

void AMWGameModeExtended::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	APlayerState const* PlayerState = NewPlayer->PlayerState;
	FUniqueNetIdRepl const UniqueNetId = PlayerState->GetUniqueId();
	
	if(UniqueNetId.IsValid() && UniqueNetId->IsValid())
	{
		if(ULocalPlayer* LocalPlayer = NewPlayer->GetLocalPlayer())
		{
			LocalPlayer->SetCachedUniqueNetId(UniqueNetId);
		}
		InitLoggedInPlayer(*UniqueNetId);
	}
}

FString AMWGameModeExtended::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void AMWGameModeExtended::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
                                   FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* AMWGameModeExtended::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	const bool bIsValidRemoteConnection = !Options.IsEmpty() && UniqueId.IsValid();
	
	IOnlineSubsystem const* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem && bIsValidRemoteConnection)
	{
		
		const FString Name = UGameplayStatics::ParseOption(Options, "Name");
		const IOnlineIdentityPtr OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
		if(OnlineIdentity.IsValid())
		{
			auto b = OnlineIdentity->Login(-1,
				FOnlineAccountCredentials("", Name, UniqueId->ToString()));
		}
	}
	return PlayerController;
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

void AMWGameModeExtended::InitLoggedInPlayer(const FUniqueNetId& UserId) const
{
	IOnlineSubsystem const* OnlineSubsystem = IOnlineSubsystem::Get();
	if(!UserId.IsValid() || !OnlineSubsystem)
	{
		return;
	}
	
	// Initialize login related data for local user
	// this needs to be better organized
	if(OnlineSubsystem->IsLocalPlayer(UserId))
	{
		
		TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if(IsValid(PlayerController))
		{
			PlayerController->PlayerState->SetUniqueId((FUniqueNetIdRepl(UserId)));
			FUniqueNetIdRepl UniqueIdRepl = PlayerController->PlayerState->GetUniqueId();
			PlayerController->GetLocalPlayer()->SetCachedUniqueNetId(UniqueIdRepl);
			UE_LOG_ONLINE_IDENTITY(Display, TEXT("Local player logged in with id: %s"), *UniqueIdRepl->ToString());
			// GameSession->RegisterPlayer(PlayerController, ui, false);
		}
	}
	// Handle voice registration
	if(IOnlineVoicePtr const OnlineVoice = OnlineSubsystem->GetVoiceInterface())
	{
		if(OnlineSubsystem->IsLocalPlayer(UserId))
		{
			// This is a local player. In case their PlayerState came last during replication, reprocess muting
			OnlineVoice->RegisterLocalTalker(0);
			OnlineVoice->ProcessMuteChangeNotification();
		}
		else
		{
			OnlineVoice->RegisterRemoteTalker(UserId);
		}
	}
}

void AMWGameModeExtended::OnOnlineSubsystemLogin(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) const
{
	if(bWasSuccessful && UserId.IsValid())
		InitLoggedInPlayer(UserId);
}

