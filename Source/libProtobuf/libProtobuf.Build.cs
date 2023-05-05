using UnrealBuildTool;
using System.IO;

public class libProtobuf : ModuleRules {
	public libProtobuf(ReadOnlyTargetRules Target) : base(Target) {
		Type = ModuleType.External;

        PublicDefinitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
        PublicDefinitions.Add("GOOGLE_PROTOBUF_CMAKE_BUILD");

        bEnableUndefinedIdentifierWarnings = false;

        PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

		if (Target.Platform == UnrealTargetPlatform.Win64) {
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Windows", "libprotobuf.lib"));
		} else if (Target.Platform == UnrealTargetPlatform.IOS) {
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "IOS", "libprotobuf.a"));
		} else if (Target.Platform == UnrealTargetPlatform.Android) {
			PublicDefinitions.Add("HAVE_PTHREAD");
			
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Android", "ARMv7", "libprotobuf.a"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Android", "ARM64", "libprotobuf.a"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Android", "x64", "libprotobuf.a"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Android", "x86", "libprotobuf.a"));
		}
	}
}
