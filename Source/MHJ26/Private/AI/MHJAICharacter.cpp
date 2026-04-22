// Copyright (c) 2026 Hasmile


#include "AI/MHJAICharacter.h"

AMHJAICharacter::AMHJAICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	bEnableSight = true;
	SightRadius = 10000.0f;
	SightLossRadius = 18000.0f;
	PeripheralRange = 90.0f;
	SightMemory = 0.1f;
	bEnableHearing = true;
	HearingRange = 2400.0f;
}
