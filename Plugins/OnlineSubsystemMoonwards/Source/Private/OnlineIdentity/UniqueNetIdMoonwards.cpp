#include "OnlineIdentity/UniqueNetIdMoonwards.h"

#include "OSMWCommon.h"

FUniqueNetIdMoonwards::FUniqueNetIdMoonwards(const FString& UserId): UserId(UserId)
{
	UserIdBytes.AddUninitialized( UserId.Len());
	// UserIdBytes = new uint8[UserIdByteSize];
	StringToBytes(UserId, UserIdBytes.GetData(), UserId.Len());
}

FUniqueNetIdMoonwards::FUniqueNetIdMoonwards(const FUniqueNetId& Src): FUniqueNetId(Src)
{
}

FName FUniqueNetIdMoonwards::GetType() const
{
	return MOONWARDS_SUBSYSTEM;
}

const uint8* FUniqueNetIdMoonwards::GetBytes() const
{
	return UserIdBytes.GetData();
}

int32 FUniqueNetIdMoonwards::GetSize() const
{
	return UserId.Len();
}

bool FUniqueNetIdMoonwards::IsValid() const
{
	return !UserId.IsEmpty();
	// return true;
}

FString FUniqueNetIdMoonwards::ToString() const
{
	return UserId;
}

FString FUniqueNetIdMoonwards::ToDebugString() const
{
	return MOONWARDS_SUBSYSTEM.ToString() + ":" + UserId;
}

bool FUniqueNetIdMoonwards::Compare(const FUniqueNetId& Other) const
{
	return UserId == Other.ToString();
}
