// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class DorothysJob : ModuleRules
{
    public DorothysJob(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "DirtAnalysis",
            "Niagara",
            "EngineCameras",
            "MediaAssets"
        });

        AddEngineThirdPartyPrivateStaticDependencies(Target, "Steamworks");

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore",
            "FMODStudio",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "OnlineSubsystemNull",
            "OnlineSubsystemSteam"
        });
    }
}
