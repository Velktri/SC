using UnrealBuildTool;

public class SCLoadingScreen : ModuleRules
{
    public SCLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "MoviePlayer",
                "Slate",
                "SlateCore",
                "InputCore",
                "SC"
            }
        );

        PublicIncludePaths.AddRange(
            new string[] {
                "SCLoadingScreen/Public"
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
                "SCLoadingScreen/Private"
            }
        );
    }
}