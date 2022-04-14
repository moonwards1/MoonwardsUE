// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineIdentity/OnlineUserAccountMoonwards.h"

FUserOnlineAccountMoonwards::FUserOnlineAccountMoonwards(FString& AccessToken, FUniqueNetIdMoonwardsRef UniqueId) : UniqueNetId(UniqueId)
{
	AuthAttributes.Add(AUTH_ATTR_ID_TOKEN, AccessToken);
	this->AccessToken = AccessToken;
}

FUserOnlineAccountMoonwards::~FUserOnlineAccountMoonwards()
{
}

FUniqueNetIdRef FUserOnlineAccountMoonwards::GetUserId() const
{
	return UniqueNetId;
}

FString FUserOnlineAccountMoonwards::GetRealName() const
{
	FString RealName;
	GetUserAttribute(USER_ATTR_REALNAME, RealName);
	return RealName;
}

FString FUserOnlineAccountMoonwards::GetDisplayName(const FString& Platform) const
{
	FString DisplayName;
	GetUserAttribute(USER_ATTR_DISPLAYNAME, DisplayName);
	return DisplayName;
}

bool FUserOnlineAccountMoonwards::GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const
{
	OutAttrValue = *UserAttributes.Find(AttrName);
	if(OutAttrValue.IsEmpty())
		return false;
	return true;
}

bool FUserOnlineAccountMoonwards::SetUserLocalAttribute(const FString& AttrName, const FString& InAttrValue)
{
	return FUserOnlineAccount::SetUserLocalAttribute(AttrName, InAttrValue);
}

FString FUserOnlineAccountMoonwards::GetAccessToken() const
{
	return AccessToken;
}

bool FUserOnlineAccountMoonwards::GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const
{
	OutAttrValue = *AuthAttributes.Find(AttrName);
	if(OutAttrValue.IsEmpty())
		return false;
	return true;
}

bool FUserOnlineAccountMoonwards::SetUserAttribute(const FString& AttrName, const FString& AttrValue)
{
	UserAttributes.Add(AttrName, AttrValue);
	return true;
}
