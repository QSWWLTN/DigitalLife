// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class T : ModuleRules
{
	public T(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDefinitions.Add("NO_PROTOBUF=1");

        PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
            "ApplicationCore",
            "Engine",
			"InputCore",
			"Json",
            "JsonUtilities",
            "Sockets",
            "Networking",
            "SlateCore",
            "UMG",
            "libProtobuf"
        });

        bEnableUndefinedIdentifierWarnings = false;

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Launch"
			}
            );
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "Test.UPL.XML"));
        }
    }
}
