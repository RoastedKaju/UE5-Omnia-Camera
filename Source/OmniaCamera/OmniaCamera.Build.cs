// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OmniaCamera : ModuleRules
{
	public OmniaCamera(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "GameplayTags" });
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore" });
	}
}