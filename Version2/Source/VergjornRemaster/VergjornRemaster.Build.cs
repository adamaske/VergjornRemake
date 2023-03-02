// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VergjornRemaster : ModuleRules
{
	public VergjornRemaster(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "Json" });
    }
}
