// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "UniqueNetIdMoonwards.h"
/**
 * 
 */
class ONLINESUBSYSTEMMOONWARDS_API FUserOnlineAccountMoonwards : public FUserOnlineAccount
{
private:
	FUniqueNetIdMoonwardsRef UniqueNetId;
	FString AccessToken;

	TMap<FString, FString> UserAttributes;
	TMap<FString, FString> AuthAttributes;
	
public:
	FUserOnlineAccountMoonwards(FString const& AccessToken, FUniqueNetIdMoonwardsRef UserId);
	virtual ~FUserOnlineAccountMoonwards() override;
	
	virtual FUniqueNetIdRef GetUserId() const override;
	virtual FString GetRealName() const override;
	virtual FString GetDisplayName(const FString& Platform) const override;
	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override;
	virtual bool SetUserLocalAttribute(const FString& AttrName, const FString& InAttrValue) override;
	virtual FString GetAccessToken() const override;
	virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override;
	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override;

	
};

typedef TSharedRef<FUserOnlineAccountMoonwards> FUserOnlineAccountMoonwardsRef;
typedef TSharedPtr<FUserOnlineAccountMoonwards> FUserOnlineAccountMoonwardsPtr;