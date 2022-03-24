#pragma once
#include "OnlineSubsystemTypes.h"

UENUM(BlueprintType)
enum ELoginStatusOss
{
	// Unreal cries if 0 isn't defined.
	INVALID = 0,
	NotLoggedIn = ELoginStatus::Type::NotLoggedIn,
	UsingLocalProfile = ELoginStatus::Type::UsingLocalProfile,
	LoggedIn = ELoginStatus::Type::LoggedIn
};
