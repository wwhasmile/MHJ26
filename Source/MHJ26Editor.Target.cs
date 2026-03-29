// Copyright (c) 2026 Hasmile

using UnrealBuildTool;
using System.Collections.Generic;

public class MHJ26EditorTarget : TargetRules
{
	public MHJ26EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "MHJ26" } );
	}
}
