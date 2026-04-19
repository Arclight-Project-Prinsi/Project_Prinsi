// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Prinsi : ModuleRules
{
	public Project_Prinsi(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Project_Prinsi",
			"Project_Prinsi/Variant_Strategy",
			"Project_Prinsi/Variant_Strategy/UI",
			"Project_Prinsi/Variant_TwinStick",
			"Project_Prinsi/Variant_TwinStick/AI",
			"Project_Prinsi/Variant_TwinStick/Gameplay",
			"Project_Prinsi/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
