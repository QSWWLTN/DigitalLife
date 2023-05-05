
using UnrealBuildTool;

public class VRM4UCapture : ModuleRules
{
	public VRM4UCapture(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
                "InputCore",
                "ApplicationCore",
                "Engine",
				"Json",
                "Slate",
                "SlateCore",
                "RenderCore",
                "RHI",
                "Renderer",
                "Networking",
                "Sockets",

                "VRM4U",
            });

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
            });

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
			});

        PrivateIncludePaths.AddRange(
        new string[] {
			// Relative to Engine\Plugins\Runtime\Oculus\OculusVR\Source
			//"../Runtime/Renderer/Private",
        });
    }
}
