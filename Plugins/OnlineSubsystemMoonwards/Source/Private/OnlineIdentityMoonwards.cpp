#include "OnlineIdentityMoonwards.h"

#include "APIModel/LoginRequestData.h"
#include "JsonObjectConverter.h"
#include "OSMWCommon.h"


FOnlineIdentityMoonwards::FOnlineIdentityMoonwards()
{
	Http = &FHttpModule::Get();
}

bool FOnlineIdentityMoonwards::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	//@TODO: validate and return false if credentials aren't valid.
	
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindRaw(this, &FOnlineIdentityMoonwards::OnLoginRequestCompleted);
	//This is the url on which to process the request
	Request->SetURL(MOONWARDS_API_URL + "login/" + AccountCredentials.Token);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
	return true;
}

bool FOnlineIdentityMoonwards::Logout(int32 LocalUserNum)
{
	LoginStatus = ELoginStatus::Type::NotLoggedIn;

	TriggerOnLoginChangedDelegates(0);
	TriggerOnLogoutCompleteDelegates(0, true);
	return true;
}

TSharedPtr<FUserOnlineAccount> FOnlineIdentityMoonwards::GetUserAccount(const FUniqueNetId& UserId) const
{
	return nullptr;
}

bool FOnlineIdentityMoonwards::AutoLogin(int32 LocalUserNum)
{
	return false;
}

TArray<TSharedPtr<FUserOnlineAccount>> FOnlineIdentityMoonwards::GetAllUserAccounts() const
{
	return TArray<TSharedPtr<FUserOnlineAccount>>();

}

FUniqueNetIdPtr FOnlineIdentityMoonwards::GetUniquePlayerId(int32 LocalUserNum) const
{
	// return MakeShareable<FUniqueNetId>(UniqueNetId);
	return nullptr;
}

FUniqueNetIdPtr FOnlineIdentityMoonwards::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	return nullptr;

}

FUniqueNetIdPtr FOnlineIdentityMoonwards::CreateUniquePlayerId(const FString& Str)
{
	return nullptr;

}

ELoginStatus::Type FOnlineIdentityMoonwards::GetLoginStatus(int32 LocalUserNum) const
{
	return LoginStatus;
}

ELoginStatus::Type FOnlineIdentityMoonwards::GetLoginStatus(const FUniqueNetId& UserId) const
{
	return LoginStatus;
}

FString FOnlineIdentityMoonwards::GetPlayerNickname(int32 LocalUserNum) const
{
	return DisplayName;
}

FString FOnlineIdentityMoonwards::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	return DisplayName;
}

FString FOnlineIdentityMoonwards::GetAuthToken(int32 LocalUserNum) const
{
	return "";
}

void FOnlineIdentityMoonwards::RevokeAuthToken(const FUniqueNetId& LocalUserId,
	const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
}

void FOnlineIdentityMoonwards::GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege,
	const FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
}

FPlatformUserId FOnlineIdentityMoonwards::GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueId) const
{
	return 0;
}

FString FOnlineIdentityMoonwards::GetAuthType() const
{
	return "";
}

void FOnlineIdentityMoonwards::OnLoginRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FLoginRequestData res {};
	if(!bWasSuccessful)
	{
		const TSharedRef<FLoginRequestData> LoginResponse = MakeShared<FLoginRequestData>(res);
		TriggerOnLoginChangedDelegates(0);
		TriggerOnLoginCompleteDelegates(0, false, UniqueNetId, "Login failed.");
	}
	
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;
    
	const FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &res, 0, 0);

	const TSharedRef<FLoginRequestData> LoginResult = MakeShared<FLoginRequestData>(res);

	UniqueNetId.UserId = LoginResult->Id;
	DisplayName = LoginResult->Username;

	LoginStatus = ELoginStatus::Type::LoggedIn;
	
	// Broadcast events using online subsystem syntax
	TriggerOnLoginChangedDelegates(0);
	TriggerOnLoginCompleteDelegates(0, true, UniqueNetId, FString() );
	
}
