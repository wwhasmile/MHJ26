// Copyright (c) 2026 Hasmile

using UnrealBuildTool;

public class MHJ26 : ModuleRules
{
	public MHJ26(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"EnhancedInput",
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"PhysicsCore",
			"AIModule",
			"SPUD"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "SlateCore", "Slate", "AnimGraphRuntime", "NavigationSystem" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
