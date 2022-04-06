#include "OnlineIdentity/OnlineIdentityMoonwards.h"

#include "APIModel/LoginRequestData.h"
#include "JsonObjectConverter.h"
#include "OSMWCommon.h"

#include "OnlineIdentity/OnlineUserAccountMoonwards.h"


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
	// Crude way to pass this over until we implement a proper server to handle all logins.
	// Request->SetHeader("LocalUserNum", FString::FromInt(LocalUserNum));
	Request->ProcessRequest();
	return true;
}

bool FOnlineIdentityMoonwards::Logout(int32 LocalUserNum)
{
	auto const UserId = UserIds.FindAndRemoveChecked(LocalUserNum);
	auto const result = UserAccounts.Remove(UserId->ToString());
	if(result > 0)
	{
		TriggerOnLoginChangedDelegates(LocalUserNum);
		TriggerOnLogoutCompleteDelegates(LocalUserNum, true);
		return true;
	}
	else
	{
		TriggerOnLogoutCompleteDelegates(LocalUserNum, false);
		return false;
	}
}

TSharedPtr<FUserOnlineAccount> FOnlineIdentityMoonwards::GetUserAccount(const FUniqueNetId& UserId) const
{
	const TSharedRef<FUserOnlineAccountMoonwards>* FoundUserAccount = UserAccounts.Find(UserId.ToString());
	if (FoundUserAccount != nullptr)
	{
		TSharedPtr<FUserOnlineAccount> Result = *FoundUserAccount;
		return Result;
	}

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
	auto const FoundId = UserIds.Find(LocalUserNum);
	if(FoundId)
		return *FoundId;
	return nullptr;
}

FUniqueNetIdPtr FOnlineIdentityMoonwards::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	return nullptr;
}

FUniqueNetIdPtr FOnlineIdentityMoonwards::CreateUniquePlayerId(const FString& Str)
{
	return MakeShareable( new FUniqueNetIdMoonwards(Str));
}

ELoginStatus::Type FOnlineIdentityMoonwards::GetLoginStatus(int32 LocalUserNum) const
{
	FUniqueNetIdPtr const UserId = GetUniquePlayerId(LocalUserNum);
	if (UserId.IsValid())
	{
		return GetLoginStatus(*UserId);
	}
	return ELoginStatus::NotLoggedIn;
}

ELoginStatus::Type FOnlineIdentityMoonwards::GetLoginStatus(const FUniqueNetId& UserId) const
{
	TSharedPtr<FUserOnlineAccount> const UserAccount = GetUserAccount(UserId);
	if (UserAccount.IsValid() &&
		UserAccount->GetUserId()->IsValid() &&
		!UserAccount->GetAccessToken().IsEmpty())
	{
		return ELoginStatus::LoggedIn;
	}
	return ELoginStatus::NotLoggedIn;
}

FString FOnlineIdentityMoonwards::GetPlayerNickname(int32 LocalUserNum) const
{
	FUniqueNetIdPtr const UserId = GetUniquePlayerId(LocalUserNum);
	auto const UserAccount = GetUserAccount(*UserId);
	return UserAccount->GetDisplayName();
}

FString FOnlineIdentityMoonwards::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	return GetUserAccount(UserId)->GetDisplayName();
}

FString FOnlineIdentityMoonwards::GetAuthToken(int32 LocalUserNum) const
{
	FUniqueNetIdPtr const UserId = GetUniquePlayerId(LocalUserNum);
	return GetUserAccount(*UserId)->GetAccessToken();
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
	FLoginRequestData LoginResult {};

	if(!bWasSuccessful)
	{
		FUniqueNetIdMoonwardsRef const UniqueNetId = MakeShared<FUniqueNetIdMoonwards>();
		const TSharedRef<FLoginRequestData> LoginResponse = MakeShared<FLoginRequestData>(LoginResult);
		TriggerOnLoginChangedDelegates(0);
		TriggerOnLoginCompleteDelegates(0, false, UniqueNetId.Get(), "Login failed.");
	}
	
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;
    // const int32 LocalUserNum = FCString::Atoi( ToCStr(Request->GetHeader("LocalUserNum")));
	const FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &LoginResult, 0, 0);

	FUniqueNetIdMoonwardsRef const UniqueNetId = MakeShared<FUniqueNetIdMoonwards>(LoginResult.Id);

	FUserOnlineAccountMoonwardsRef const UserAccount = MakeShared<FUserOnlineAccountMoonwards>(LoginResult.Id, UniqueNetId.Get());
	UserAccount->SetUserAttribute(USER_ATTR_DISPLAYNAME, LoginResult.Username);
	
	UserIds.Add(0, UniqueNetId);
	UserAccounts.Add(UniqueNetId->ToString(), UserAccount);

	if(!LoginResult.Id.IsEmpty())
	{
		// Broadcast events using online subsystem syntax
		TriggerOnLoginChangedDelegates(0);
		TriggerOnLoginCompleteDelegates(0, true, UniqueNetId.Get(), FString() );
	}else
	{
		TriggerOnLoginChangedDelegates(0);
		TriggerOnLoginCompleteDelegates(0, false, UniqueNetId.Get(), FString() );
	}
}
