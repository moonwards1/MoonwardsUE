#include "OnlineIdentity/OnlineIdentityMoonwards.h"

#include "APIModel/LoginRequestData.h"
#include "JsonObjectConverter.h"
#include "OnlineSubsystem.h"
#include "OSMWCommon.h"

#include "OnlineIdentity/OnlineUserAccountMoonwards.h"


FOnlineIdentityMoonwards::FOnlineIdentityMoonwards()
{
	Http = &FHttpModule::Get();
}

bool FOnlineIdentityMoonwards::Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials)
{
	if(LocalUserNum == 0)
	{
		//@TODO: validate and return false if credentials aren't valid.
		PendingLocalUserNum = LocalUserNum;
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
	}else
	{
		// temporary til we implement proper login server
		AddPlayerAsLoggedIn(LocalUserNum, AccountCredentials.Id, AccountCredentials.Token);
	}
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
	return MakeShareable(new FUniqueNetIdMoonwards(BytesToString(Bytes, Size)));
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
	// Should cache these statuses and add proper logic to handle temporary logouts etc.
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
	const TSharedPtr<FUserOnlineAccount> UserAcc = GetUserAccount(UserId);
	if(UserAcc.IsValid())
	{
		return UserAcc->GetDisplayName();
	}
	return "";
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
	return FPlatformMisc::GetPlatformUserForUserIndex(0);
}

FString FOnlineIdentityMoonwards::GetAuthType() const
{
	return "";
}

void FOnlineIdentityMoonwards::AddPlayerAsLoggedIn(int32 LocalUserNum, FString const& Username, FString const& UniqueNetIdStr)
{
	FUniqueNetIdMoonwardsRef UniqueNetId = MakeShared<FUniqueNetIdMoonwards>(UniqueNetIdStr);

	FUserOnlineAccountMoonwardsRef const UserAccount = MakeShared<FUserOnlineAccountMoonwards>(Username, UniqueNetId);
	UserAccount->SetUserAttribute(USER_ATTR_DISPLAYNAME, Username);
	
	if(UniqueNetId->IsValid())
	{
		UserIds.Add(LocalUserNum, UniqueNetId);
		UserAccounts.Add(UniqueNetId->ToString(), UserAccount);
		UE_LOG_ONLINE_IDENTITY(Display, TEXT("Added user to logged in players. Username: %s, Id: %s"), *Username, *UniqueNetIdStr);

		// TriggerOnLoginStatusChangedDelegates()
		// Broadcast events using online subsystem syntax
		TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::Type::NotLoggedIn, ELoginStatus::Type::LoggedIn, UniqueNetId.Get());
		TriggerOnLoginChangedDelegates(LocalUserNum);
		TriggerOnLoginCompleteDelegates(LocalUserNum, true, UniqueNetId.Get(), FString() );

	}else
	{
		UE_LOG_ONLINE_IDENTITY(Display, TEXT("Failed to add user to logged in players. Username: %s, Id: %s"), *Username, *UniqueNetIdStr);

		TriggerOnLoginStatusChangedDelegates(LocalUserNum, ELoginStatus::Type::NotLoggedIn, ELoginStatus::Type::NotLoggedIn, UniqueNetId.Get());
		TriggerOnLoginChangedDelegates(LocalUserNum);
		TriggerOnLoginCompleteDelegates(LocalUserNum, false, UniqueNetId.Get(), "Login failed." );
	}
}

void FOnlineIdentityMoonwards::OnLoginRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FLoginRequestData LoginResult {};

	// Clean this up
	if(!bWasSuccessful)
	{
		FUniqueNetIdMoonwardsRef const UniqueNetId = MakeShared<FUniqueNetIdMoonwards>("");
		const TSharedRef<FLoginRequestData> LoginResponse = MakeShared<FLoginRequestData>(LoginResult);
		TriggerOnLoginStatusChangedDelegates(PendingLocalUserNum, ELoginStatus::Type::NotLoggedIn, ELoginStatus::Type::NotLoggedIn, UniqueNetId.Get());
		TriggerOnLoginChangedDelegates(PendingLocalUserNum);
		TriggerOnLoginCompleteDelegates(PendingLocalUserNum, false, UniqueNetId.Get(), "Login failed." );
	}
	
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;
	const FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &LoginResult, 0, 0);

	FUniqueNetIdMoonwardsRef const UniqueNetId = MakeShared<FUniqueNetIdMoonwards>(LoginResult.Id);

	FUserOnlineAccountMoonwardsRef const UserAccount = MakeShared<FUserOnlineAccountMoonwards>(LoginResult.Id, UniqueNetId);
	UserAccount->SetUserAttribute(USER_ATTR_DISPLAYNAME, LoginResult.Username);

	AddPlayerAsLoggedIn(PendingLocalUserNum, LoginResult.Username, LoginResult.Id);
}
