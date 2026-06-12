using UnrealBuildTool;

public class VergjornBuild : ModuleRules
{
	public VergjornBuild(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"VergjornCore",
			"VergjornSim",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayMessageRuntime",
		});
	}
}
