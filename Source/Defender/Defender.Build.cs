// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Defender : ModuleRules
{
	public Defender(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AnimGraphRuntime", "UMG", "NavigationSystem", "AIModule", "GameplayTasks", "SlateCore", "Json", "JsonUtilities", "Paper2D", "Niagara", "LevelSequence" , "MovieScene" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
