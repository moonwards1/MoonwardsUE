#include "OnlineIdentity/UniqueNetIdMoonwards.h"

#include "OSMWCommon.h"

FName FUniqueNetIdMoonwards::GetType() const
{
	return MOONWARDS_SUBSYSTEM;
}

const uint8* FUniqueNetIdMoonwards::GetBytes() const
{
	return nullptr;
}

int32 FUniqueNetIdMoonwards::GetSize() const
{
	return UserId.Len();
}

bool FUniqueNetIdMoonwards::IsValid() const
{
	return !UserId.IsEmpty();
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
