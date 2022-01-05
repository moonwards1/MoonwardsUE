#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MWLogger.generated.h"

// Calls: URTLogger::LogMessage(FString Message, ERTLogCategory Category, ERTMessageSeverity Severity, bLogToViewport);
#define MW_LOG(Message, ...) UMWLogger::LogMessage(Message, __FUNCTION__, ## __VA_ARGS__);


// Matches EMessageSeverity::Type, but we can extend it further if need be.
UENUM(BlueprintType)
enum class EMWLogMessageSeverity : uint8 {
	// Game can not recover (This will crash the game)
	Fatal = 0,
	// Recoverable error
	Error = 1,
	Warning = 3,
	Info = 4,
};

// Categories remain to be actually implemented.
// @TODO: Make these appear in their own tabs, and append necessary indicative text when logged to file.
UENUM(BlueprintType)
enum class EMWLogCategory : uint8
{
	// Does not have a category
	Other,
	Online,
	// Game / gameplay framework related
	Game,
	Rendering,
	Audio,
	UI,
};

UCLASS()
class MOONWARDSLOGGER_API UMWLogger : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Log a message and includes the blueprint call stack as tokenized messages
	UFUNCTION(BlueprintCallable, Category = "Moonwarsd Logger", meta = (HidePin = "Caller", DefaultToSelf = "Caller"))
	static void LogMessage(FString const Message, UObject const* Caller, EMWLogCategory const LogCategory,
		EMWLogMessageSeverity const Severity = EMWLogMessageSeverity::Info, bool const bLogToViewport = false);

	// Log a message from C++ code
	static void LogMessage(FString Message, FString CallerName, EMWLogCategory LogCategory = EMWLogCategory::Other, EMWLogMessageSeverity Severity = EMWLogMessageSeverity::Info,
		bool bLogToViewport = false);

private:
	static void OnBringToFocus(TSharedRef<IMessageToken> const& Token);
};
