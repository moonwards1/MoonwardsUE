#include "OnlineIdentity/AsyncAction/LoginRequestAsyncAction.h"

#include "Http.h"
#include "JsonObjectConverter.h"
#include "OnlineSubsystem.h"

#include "APIModel/LoginRequestData.h"

#include "Interfaces/OnlineIdentityInterface.h"


ULoginRequestAsyncAction::ULoginRequestAsyncAction(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	OnLoginRequestCompleted.BindUObject(this, &ULoginRequestAsyncAction::OnResponseReceived);
	Http = &FHttpModule::Get();
}

ULoginRequestAsyncAction* ULoginRequestAsyncAction::TryLogin(UObject* WorldContextObject,
                                                             FString const LoginToken)
{
	ULoginRequestAsyncAction* BlueprintNode = NewObject<ULoginRequestAsyncAction>();
	BlueprintNode->WorldContextObject = WorldContextObject;
	BlueprintNode->LoginToken = LoginToken;
	return BlueprintNode;
}

void ULoginRequestAsyncAction::Activate()
{
	Super::Activate();
	OnlineIdentity = IOnlineSubsystem::Get()->GetIdentityInterface();
	if(OnlineIdentity)
		TryLogin();
	else
		UE_LOG_ONLINE(Error, TEXT("Failed to use Moonwards OSS."));
}


void ULoginRequestAsyncAction::TryLogin()
{
	FOnlineAccountCredentials AccountCredentials;
	AccountCredentials.Token = LoginToken;

	OnlineIdentity->AddOnLoginCompleteDelegate_Handle(0, OnLoginRequestCompleted);
	
	OnlineIdentity->Login(0, AccountCredentials);
}

void ULoginRequestAsyncAction::OnResponseReceived(int32 LocalUserNum, bool bSuccessful, const FUniqueNetId& UserId, const FString& Error) const
{
	if(bSuccessful && !UserId.ToString().IsEmpty())
		OnSucceeded.Broadcast(LocalUserNum, UserId.ToString(), IOnlineSubsystem::Get()->GetIdentityInterface()->GetPlayerNickname(0));
	else
		OnFailed.Broadcast(LocalUserNum, UserId.ToString(), IOnlineSubsystem::Get()->GetIdentityInterface()->GetPlayerNickname(0));
}
