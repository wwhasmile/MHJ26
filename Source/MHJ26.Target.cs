// Copyright (c) 2026 Hasmile

using UnrealBuildTool;
using System.Collections.Generic;

public class MHJ26Target : TargetRules
{
	public MHJ26Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "MHJ26" } );
	}
}
