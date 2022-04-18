// 


#include "MWPlayerStateExtended.h"

#include "OnlineSubsystem.h"

#include "GameFramework/GameStateBase.h"

#include "Interfaces/VoiceInterface.h"

void AMWPlayerStateExtended::OnSetUniqueId()
{
	Super::OnSetUniqueId();
	const FUniqueNetIdRepl UniqueNetId = GetUniqueId();
	if(!GetUniqueId().IsValid() || !GetUniqueId().GetUniqueNetId().IsValid())
	{
		return;
	}
	// Handle voice registration
	// const FUniqueNetIdRepl UniqueNetId = GetUniqueId();
	
	if(IOnlineVoicePtr const OnlineVoice = IOnlineSubsystem::Get()->GetVoiceInterface())
	{
		if(IOnlineSubsystem::Get()->IsLocalPlayer(*UniqueNetId.GetUniqueNetId()))
		{
			// This is a local player. In case their PlayerState came last during replication, reprocess muting
			OnlineVoice->RegisterLocalTalker(0);
			OnlineVoice->ProcessMuteChangeNotification();
		}
		else
		{
			OnlineVoice->RegisterRemoteTalker(*UniqueNetId.GetUniqueNetId());
		}
	}
}