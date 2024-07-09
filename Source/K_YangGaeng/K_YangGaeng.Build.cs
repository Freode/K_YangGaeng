// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class K_YangGaeng : ModuleRules
{
	public K_YangGaeng(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AnimGraphRuntime", "UMG", "Slate", "SlateCore" });
    }
}
