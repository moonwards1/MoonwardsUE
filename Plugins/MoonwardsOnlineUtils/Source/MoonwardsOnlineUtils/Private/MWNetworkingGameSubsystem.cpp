// Fill out your copyright notice in the Description page of Project Settings.


#include "MWNetworkingGameSubsystem.h"

//@TODO: Add logging
void UMWNetworkingGameSubsystem::ConnectToServer(UWorld* World, FURL& Url)
{
	if(PendingNetClient)
	{
		UE_LOG(LogNet, Log, TEXT("A connection attempt is already underway."));

		OnPendingClientFinished.Broadcast(false);
		return;
	}
	
	PendingNetClient = NewObject<UMWNPendingNetClient>();
	PendingNetClient->OnConnectionSequenceCompleted.AddUObject(this, &UMWNetworkingGameSubsystem::PendingClientConnected);
	PendingNetClient->TryConnect(World, Url);
}

void UMWNetworkingGameSubsystem::PendingClientConnected(UMWNPendingNetClient* PendingClient, bool bSuccess) const
{
	UWorld* World = PendingClient->World;
	World->SetNetDriver(PendingClient->NetDriver);

	UNetDriver* NetDriver = PendingClient->NetDriver;
	if (NetDriver)
	{
		// The pending net driver is renamed to the current "game net driver"
		// NetDriver->SetNetDriverName(NAME_GameNetDriver);
		// NetDriver->NetDriverName = NAME_GameNetDriver;
		// GEngine->
		
		NetDriver->SetWorld(World);
		NetDriver->StatPeriod = 1.f;
		
		FLevelCollection& SourceLevels = World->FindOrAddCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		SourceLevels.SetNetDriver(NetDriver);

		FLevelCollection& StaticLevels = World->FindOrAddCollectionByType(ELevelCollectionType::StaticLevels);
		StaticLevels.SetNetDriver(NetDriver);
	}

	PendingClient->RequestJoin();
	GEngine->TransitionType = ETransitionType::Connecting;
	PendingClient = nullptr;
	OnPendingClientFinished.Broadcast(true);
	UE_LOG(LogNet, Log, TEXT("Successfully joined the server."));
}
