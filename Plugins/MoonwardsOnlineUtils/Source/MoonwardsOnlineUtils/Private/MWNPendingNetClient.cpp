// Fill out your copyright notice in the Description page of Project Settings.


#include "MWNPendingNetClient.h"

#include "Kismet/GameplayStatics.h"

#include "Misc/NetworkVersion.h"

#include "Net/DataChannel.h"

void UMWNPendingNetClient::TryConnect(UWorld* CurrentWorld, FURL& Url)
{
	LLM_SCOPE(ELLMTag::Networking);

	if (!GDisallowNetworkTravel)
	{
		this->World = CurrentWorld;
		// NETWORK_PROFILER(GNetworkProfiler.TrackSessionChange(true, URL));

		// Try to create network driver.
		// Currently using GameNetDriver as the pending name as well, because changing the name later is inaccessible.
		if (GEngine->CreateNamedNetDriver(World, NAME_GameNetDriver, NAME_GameNetDriver))
		{
			NetDriver = GEngine->FindNamedNetDriver(World, NAME_GameNetDriver);
			NetDriver->World = nullptr;
		}

		if (NetDriver == nullptr)
		{
			UE_LOG(LogNet, Warning, TEXT("Error initializing the pending net driver.  Check the configuration of NetDriverDefinitions and make sure module/plugin dependencies are correct."));
			ConnectionError = NSLOCTEXT("Engine", "NetworkDriverInit", "Error creating network driver.").ToString();
			return;
		}

		if( NetDriver->InitConnect( this, URL, ConnectionError ) )
		{
			const UNetConnection* ServerConn = NetDriver->ServerConnection;

			// Bit of a hack here to prevent driver name based code from running, since we can't actually change the name...
			NetDriver->StatPeriod = TNumericLimits<float>::Max();	
			NetDriver->World = World;
			NetDriver->TickFlush(0.01);
			NetDriver->World = nullptr;
			// FNetDelegates::OnPendingNetGameConnectionCreated.Broadcast(World);

			// Kick off the connection handshake
			if (ServerConn->Handler.IsValid())
			{
				ServerConn->Handler->BeginHandshaking(
					FPacketHandlerHandshakeComplete::CreateUObject(this, &UMWNPendingNetClient::InitiateConnectSequence));
			}
			else
			{
				InitiateConnectSequence();
			}
			bIsTickable = true;
		}
		else
		{
			// error initializing the network stack...
			UE_LOG(LogNet, Warning, TEXT("error initializing the network stack"));
			GEngine->DestroyNamedNetDriver(World, NetDriver->NetDriverName);
			NetDriver = nullptr;

			// ConnectionError should be set by calling InitConnect...however, if we set NetDriver to NULL without setting a
			// value for ConnectionError, we'll trigger the assertion at the top of UPendingNetGame::Tick() so make sure it's set
			if ( ConnectionError.Len() == 0 )
			{
				ConnectionError = NSLOCTEXT("Engine", "NetworkInit", "Error initializing network layer.").ToString();
			}
		}
	}
	else
	{
		ConnectionError = NSLOCTEXT("Engine", "UsedCheatCommands", "Console commands were used which are disallowed in netplay.  You must restart the game to create a match.").ToString();
	}
}

void UMWNPendingNetClient::NotifyControlMessage(UNetConnection* Connection, uint8 MessageType, FInBunch& Bunch)
{
	check(Connection == NetDriver->ServerConnection);

#if !UE_BUILD_SHIPPING
	UE_LOG(LogNet, Verbose, TEXT("PendingLevel received: %s"), FNetControlMessageInfo::GetName(MessageType));
#endif

	// This client got a response from the server.
	switch (MessageType)
	{
		case NMT_Upgrade:
			// Report mismatch.
			uint32 RemoteNetworkVersion;

			if (FNetControlMessage<NMT_Upgrade>::Receive(Bunch, RemoteNetworkVersion))
			{
				// Upgrade
				ConnectionError = NSLOCTEXT("Engine", "ClientOutdated", "The match you are trying to join is running an incompatible version of the game.  Please try upgrading your game version.").ToString();
				GEngine->BroadcastNetworkFailure(NULL, NetDriver, ENetworkFailure::OutdatedClient, ConnectionError);
				OnConnectionSequenceCompleted.Broadcast(this, false);
			}

			break;

		case NMT_Failure:
		{
			// our connection attempt failed for some reason, for example a synchronization mismatch (bad GUID, etc) or because the server rejected our join attempt (too many players, etc)
			// here we can further parse the string to determine the reason that the server closed our connection and present it to the user

			FString ErrorMsg;

			if (FNetControlMessage<NMT_Failure>::Receive(Bunch, ErrorMsg))
			{
				if (ErrorMsg.IsEmpty())
				{
					ErrorMsg = NSLOCTEXT("NetworkErrors", "GenericPendingConnectionFailed", "Pending Connection Failed.").ToString();
				}

				// This error will be resolved in TickWorldTravel()
				ConnectionError = ErrorMsg;

				// Force close the session
				UE_LOG(LogNet, Log, TEXT("NetConnection::Close() [%s] [%s] [%s] from NMT_Failure %s"),
					Connection->Driver ? *Connection->Driver->NetDriverName.ToString() : TEXT("NULL"),
					Connection->PlayerController ? *Connection->PlayerController->GetName() : TEXT("NoPC"),
					Connection->OwningActor ? *Connection->OwningActor->GetName() : TEXT("No Owner"),
					*ConnectionError);

				Connection->Close(ENetCloseResult::Cleanup);
				OnConnectionSequenceCompleted.Broadcast(this, false);
			}

			break;
		}
		case NMT_Challenge:
		{
			// Challenged by server.
			if (FNetControlMessage<NMT_Challenge>::Receive(Bunch, Connection->Challenge))
			{
				FURL PartialURL(URL);
				PartialURL.Host = TEXT("");
				PartialURL.Port = PartialURL.UrlConfig.DefaultPort; // HACK: Need to fix URL parsing 
				PartialURL.Map = TEXT("");

				for (int32 i = URL.Op.Num() - 1; i >= 0; i--)
				{
					if (URL.Op[i].Left(5) == TEXT("game="))
					{
						URL.Op.RemoveAt(i);
					}
				}

				ULocalPlayer* LocalPlayer = GEngine->GetFirstGamePlayer(World);
				if (LocalPlayer)
				{
					// Send the player nickname if available
					FString OverrideName = LocalPlayer->GetNickname();
					if (OverrideName.Len() > 0)
					{
						PartialURL.AddOption(*FString::Printf(TEXT("Name=%s"), *OverrideName));
					}

					// Send any game-specific url options for this player
					FString GameUrlOptions = LocalPlayer->GetGameLoginOptions();
					if (GameUrlOptions.Len() > 0)
					{
						PartialURL.AddOption(*FString::Printf(TEXT("%s"), *GameUrlOptions));
					}

					// Send the player unique Id at login
					Connection->PlayerId = LocalPlayer->GetPreferredUniqueNetId();
				}

				// Send the player's online platform name
				FName OnlinePlatformName = NAME_None;
				if (const FWorldContext* const WorldContext = GEngine->GetWorldContextFromWorld(World))
				{
					if (WorldContext->OwningGameInstance)
					{
						OnlinePlatformName = WorldContext->OwningGameInstance->GetOnlinePlatformName();
					}
				}

				Connection->ClientResponse = TEXT("0");
				FString URLString(PartialURL.ToString());
				FString OnlinePlatformNameString = OnlinePlatformName.ToString();

				FNetControlMessage<NMT_Login>::Send(Connection, Connection->ClientResponse, URLString, Connection->PlayerId, OnlinePlatformNameString);
				NetDriver->ServerConnection->FlushNet();
			}
			else
			{
				Connection->Challenge.Empty();
			}

			break;
		}
		case NMT_Welcome:
		{
			// Server accepted connection.
			FString GameName;
			FString RedirectURL;

			if (FNetControlMessage<NMT_Welcome>::Receive(Bunch, URL.Map, GameName, RedirectURL))
			{
				//GEngine->NetworkRemapPath(this, URL.Map);

				UE_LOG(LogNet, Log, TEXT("Welcomed by server (Level: %s, Game: %s)"), *URL.Map, *GameName);

				// extract map name and options
				{
					FURL DefaultURL;
					FURL TempURL(&DefaultURL, *URL.Map, TRAVEL_Partial);
					URL.Map = TempURL.Map;
					URL.RedirectURL = RedirectURL;
					URL.Op.Append(TempURL.Op);
				}

				if (GameName.Len() > 0)
				{
					URL.AddOption(*FString::Printf(TEXT("game=%s"), *GameName));
				}

				// Send out netspeed now that we're connected
				FNetControlMessage<NMT_Netspeed>::Send(Connection, Connection->CurrentNetSpeed);

				// We have successfully connected
				// TickWorldTravel will load the map and call LoadMapCompleted which eventually calls SendJoin
				bSuccessfullyConnected = true;

				// Notify listeners that we've successfully connected
				OnConnectionSequenceCompleted.Broadcast(this, true);
			}
			else
			{
				URL.Map.Empty();
			}

			break;
		}
		case NMT_NetGUIDAssign:
		{
			FNetworkGUID NetGUID;
			FString Path;

			if (FNetControlMessage<NMT_NetGUIDAssign>::Receive(Bunch, NetGUID, Path))
			{
				NetDriver->ServerConnection->PackageMap->ResolvePathAndAssignNetGUID(NetGUID, Path);
			}

			break;
		}
		case NMT_EncryptionAck:
		{
			if (FNetDelegates::OnReceivedNetworkEncryptionAck.IsBound())
			{
				TWeakObjectPtr<UNetConnection> WeakConnection = Connection;
				FNetDelegates::OnReceivedNetworkEncryptionAck.Execute(FOnEncryptionKeyResponse::CreateUObject(this, &UMWNPendingNetClient::FinalizeEncryptedConnection, WeakConnection));
			}
			else
			{
				// This error will be resolved in TickWorldTravel()
				ConnectionError = TEXT("No encryption ack handler");

				// Force close the session
				UE_LOG(LogNet, Warning, TEXT("%s: No delegate available to handle encryption ack, disconnecting."), *Connection->GetName());
				Connection->Close(ENetCloseResult::Cleanup);

				OnConnectionSequenceCompleted.Broadcast(this, false);
			}
			break;
		}
		default:
			UE_LOG(LogNet, Log, TEXT(" --- Unknown/unexpected message for pending level"));
			break;
	}
}

EAcceptConnection::Type UMWNPendingNetClient::NotifyAcceptingConnection()
{
	return EAcceptConnection::Reject; 
}
void UMWNPendingNetClient::NotifyAcceptedConnection( class UNetConnection* Connection )
{
}
bool UMWNPendingNetClient::NotifyAcceptingChannel( class UChannel* Channel )
{
	return false;
}

void UMWNPendingNetClient::FinalizeEncryptedConnection(const FEncryptionKeyResponse& EncryptionKeyResponse,
	TWeakObjectPtr<UNetConnection> NetConnection)
{
	UNetConnection* Connection = NetConnection.Get();
	if (Connection)
	{
		EConnectionState const ConState = Connection->GetConnectionState();
		if (ConState != USOCK_Invalid && ConState != USOCK_Closed && Connection->Driver)
		{
			if (EncryptionKeyResponse.Response == EEncryptionResponse::Success)
			{
				// handle deprecated path where only the key is set
				PRAGMA_DISABLE_DEPRECATION_WARNINGS
				if ((EncryptionKeyResponse.EncryptionData.Key.Num() > 0) && (EncryptionKeyResponse.EncryptionData.Key.Num() == 0))
				{
					FEncryptionData ResponseData = EncryptionKeyResponse.EncryptionData;
					ResponseData.Key = EncryptionKeyResponse.EncryptionData.Key;

					Connection->EnableEncryption(ResponseData);
				}
				PRAGMA_ENABLE_DEPRECATION_WARNINGS
				else
				{
					Connection->EnableEncryption(EncryptionKeyResponse.EncryptionData);
				}
			}
			else
			{
				// This error will be resolved in TickWorldTravel()
				FString ResponseStr(LexToString(EncryptionKeyResponse.Response));
				UE_LOG(LogNet, Warning, TEXT("UPendingNetGame::FinalizeEncryptedConnection: encryption failure [%s] %s"), *ResponseStr, *EncryptionKeyResponse.ErrorMsg);
				ConnectionError = TEXT("Encryption ack failure");
				Connection->Close(ENetCloseResult::Cleanup);
			}
		}
		else
		{
			// This error will be resolved in TickWorldTravel()
			UE_LOG(LogNet, Warning, TEXT("UPendingNetGame::FinalizeEncryptedConnection: connection in invalid state. %s"), *Connection->Describe());
			ConnectionError = TEXT("Connection encryption state failure");
			Connection->Close(ENetCloseResult::Cleanup);
		}
	}
	else
	{
		// This error will be resolved in TickWorldTravel()
		UE_LOG(LogNet, Warning, TEXT("UPendingNetGame::FinalizeEncryptedConnection: Connection is null."));
		ConnectionError = TEXT("Connection missing during encryption ack");
	}
}

void UMWNPendingNetClient::InitiateConnectSequence() const
{
	if (NetDriver != nullptr)
	{
		UNetConnection* ServerConn = NetDriver->ServerConnection;

		if (ServerConn != nullptr)
		{
			uint8 IsLittleEndian = uint8(PLATFORM_LITTLE_ENDIAN);
			check(IsLittleEndian == !!IsLittleEndian); // should only be one or zero
			
			uint32 LocalNetworkVersion = FNetworkVersion::GetLocalNetworkVersion();

			UE_LOG(LogNet, Log, TEXT( "UMWNPendingNetGame::SendInitialJoin: Sending hello. %s" ), *ServerConn->Describe());

			FString EncryptionToken;
			if (CVarNetAllowEncryption.GetValueOnGameThread() != 0)
			{
				EncryptionToken = URL.GetOption(TEXT("EncryptionToken="), TEXT(""));
			}

			FNetControlMessage<NMT_Hello>::Send(ServerConn, IsLittleEndian, LocalNetworkVersion, EncryptionToken);

			ServerConn->FlushNet();
		}
	}
}

void UMWNPendingNetClient::Tick(float DeltaTime)
{
	if(!bIsTickable)
		return;
	
	if(!NetDriver || !NetDriver->ServerConnection)
		return;
	check(NetDriver && NetDriver->ServerConnection);

	// The following line disables checks for nullptr access on NetDriver. We have checked() it's validity above,
	// but the TickDispatch call below may invalidate the ptr, thus we must null check after calling TickDispatch.
	// PVS-Studio notes that we have used the pointer before null checking (it currently does not understand check)
	//-V:NetDriver<<:522

	// Handle timed out or failed connection.
	if (NetDriver->ServerConnection->GetConnectionState() == USOCK_Closed && ConnectionError == TEXT(""))
	{
		ConnectionError = NSLOCTEXT("Engine", "ConnectionFailed", "Your connection to the host has been lost.").ToString();
		return;
	}

	/**
	 *   Update the network driver
	 *   ****may NULL itself via CancelPending if a disconnect/error occurs****
	 */
	NetDriver->TickDispatch(DeltaTime);

	if (NetDriver)
	{
		NetDriver->PostTickDispatch();
	}

	if (NetDriver)
	{
		NetDriver->TickFlush(DeltaTime);
	}

	if (NetDriver)
	{
		NetDriver->PostTickFlush();
	}
}

TStatId UMWNPendingNetClient::GetStatId() const
{
	return TStatId();
}

bool UMWNPendingNetClient::IsTickable() const
{
	return bIsTickable;
}

void UMWNPendingNetClient::RequestJoin() const
{
	APlayerController* Controller = World->GetFirstPlayerController();
	APawn* Pawn = Controller->GetPawn();
	if(IsValid(Pawn))
	{
		Controller->UnPossess();
		Pawn->Destroy();
	}
	
	FNetControlMessage<NMT_Join>::Send(NetDriver->ServerConnection);
	NetDriver->ServerConnection->FlushNet(true);
}
