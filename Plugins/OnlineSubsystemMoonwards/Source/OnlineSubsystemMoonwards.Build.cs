// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineSubsystemMoonwards : ModuleRules
{
	public OnlineSubsystemMoonwards(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"OnlineSubsystem",
				"OnlineSubsystemUtils",
				"HTTP", 
				"Json", 
				"JsonUtilities",
                "CoreOnline",
                "NetCore"

				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore", 
				"SignalProcessing",
				"AudioMixerCore",
				"Voice",
				"AudioMixer"

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
