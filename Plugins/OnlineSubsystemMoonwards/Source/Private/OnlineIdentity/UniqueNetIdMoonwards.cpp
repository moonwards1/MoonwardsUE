#include "OnlineIdentity/UniqueNetIdMoonwards.h"

#include "OSMWCommon.h"

FUniqueNetIdMoonwards::FUniqueNetIdMoonwards(const FString& UserId): UserId(UserId)
{
	UserIdByteSize = UserId.Len();
	UserIdBytes = new uint8[UserIdByteSize];
	StringToBytes(UserId, UserIdBytes, UserIdByteSize);
}

FName FUniqueNetIdMoonwards::GetType() const
{
	return MOONWARDS_SUBSYSTEM;
}

const uint8* FUniqueNetIdMoonwards::GetBytes() const
{
	return new uint8[1] {0};
}

int32 FUniqueNetIdMoonwards::GetSize() const
{
	return UserId.Len();
}

bool FUniqueNetIdMoonwards::IsValid() const
{
	// return !UserId.IsEmpty();
	return true;
}

FString FUniqueNetIdMoonwards::ToString() const
{
	return UserId;
}

FString FUniqueNetIdMoonwards::ToDebugString() const
{
	return MOONWARDS_SUBSYSTEM.ToString() + "_" + UserId;
}

bool FUniqueNetIdMoonwards::Compare(const FUniqueNetId& Other) const
{
	return UserId == Other.ToString();
}
