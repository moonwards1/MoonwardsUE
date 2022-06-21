#include "MoonwardsLogger.h"

#include "MessageLog/Public/MessageLogModule.h"
#include "MessageLog/Public/MessageLogInitializationOptions.h"

void FMoonwardsLoggerModule::StartupModule()
{
#if WITH_EDITOR
	// Check MessageLog Module is loaded
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowPages = true;
	InitOptions.bAllowClear = true;
	InitOptions.bShowFilters = true;
	InitOptions.bShowInLogWindow = true;
	// Create a new log listing with our module's name
	MessageLogModule.RegisterLogListing("MoonwardsLog", NSLOCTEXT("Moonwards Log", "MoonwardsLogLabel", "Moonwards Log"), InitOptions);
#endif
}

void FMoonwardsLoggerModule::ShutdownModule()
{
#if WITH_EDITOR

	if (FModuleManager::Get().IsModuleLoaded("MoonwardsLog"))
	{
		// unregister message log
		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
		MessageLogModule.UnregisterLogListing("MoonwardsLog");
	}
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMoonwardsLoggerModule, MoonwardsLogger)