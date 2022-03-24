﻿// #pragma once
// #ifndef OnlineIdentityInterfaceMW.h_H_
// #define OnlineIdentityInterfaceMW.h_H_
// 
// #endif

#pragma once
#include "Http.h"

#include "Interfaces/OnlineIdentityInterface.h"

#include "OnlineIdentity/UniqueNetIdMoonwards.h"

class FOnlineIdentityMoonwards : public IOnlineIdentity
{
PACKAGE_SCOPE:

	FOnlineIdentityMoonwards();
	
public:
	virtual ~FOnlineIdentityMoonwards() override {};
	
protected:
	FString DisplayName;
	FHttpModule* Http = nullptr;
	ELoginStatus::Type LoginStatus = ELoginStatus::Type::NotLoggedIn;
	FUniqueNetIdMoonwards UniqueNetId;
	
public:
	virtual bool Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials) override;
	virtual bool Logout(int32 LocalUserNum) override;
	virtual TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
	virtual bool AutoLogin(int32 LocalUserNum) override;
	virtual TArray<TSharedPtr<FUserOnlineAccount>> GetAllUserAccounts() const override;
	virtual FUniqueNetIdPtr GetUniquePlayerId(int32 LocalUserNum) const override;
	virtual FUniqueNetIdPtr CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
	virtual FUniqueNetIdPtr CreateUniquePlayerId(const FString& Str) override;
	virtual ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override;
	virtual ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
	virtual FString GetPlayerNickname(int32 LocalUserNum) const override;
	virtual FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
	virtual FString GetAuthToken(int32 LocalUserNum) const override;
	virtual void RevokeAuthToken(const FUniqueNetId& LocalUserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate) override;
	virtual void GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate) override;
	virtual FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const override;
	virtual FString GetAuthType() const override;

private:
	void OnLoginRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};

// typedef TSharedPtr<FOnlineIdentityMoonwards, ESPMode::ThreadSafe> FOnlineIdentityMoonwardsPtr;