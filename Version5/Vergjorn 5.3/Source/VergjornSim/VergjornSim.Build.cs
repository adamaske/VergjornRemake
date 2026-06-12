using UnrealBuildTool;

public class VergjornSim : ModuleRules
{
	public VergjornSim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"VergjornCore",
			"MassEntity",
			"MassCommon",
			"MassActors",
			"StructUtils",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayMessageRuntime",
			"MassSignals",
			"NavigationSystem",
		});
	}
}
