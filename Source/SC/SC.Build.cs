// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SC : ModuleRules
{
	public SC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[] {
                "SC/Private/UI/Widgets",
                "SC/Private/UI/Styles"
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RTSCamera",
                "Slate",
                "SlateCore"
            }
        );

		PrivateDependencyModuleNames.AddRange(
            new string[] {
                "RTSCamera",
                "Slate",
                "SlateCore"
            }
        );

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
