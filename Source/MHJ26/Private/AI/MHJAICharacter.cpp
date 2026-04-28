// Copyright (c) 2026 Hasmile


#include "AI/MHJAICharacter.h"

#include "BrainComponent.h"
#include "AI/MHJAIController.h"

AMHJAICharacter::AMHJAICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	bAutoStartThinking = true;
	bEnableSight = true;
	SightRadius = 10000.0f;
	SightLossRadius = 18000.0f;
	PeripheralRange = 90.0f;
	SightMemory = 0.1f;
	bEnableHearing = true;
	HearingRange = 2400.0f;
}

void AMHJAICharacter::StartThinking()
{
	AMHJAIController* AIController = Cast<AMHJAIController>(GetController());
	if (!AIController)
	{
		return;
	}
	
	AIController->StartThinking();
}

void AMHJAICharacter::StopThinking()
{
	AMHJAIController* AIController = Cast<AMHJAIController>(GetController());
	if (!AIController)
	{
		return;
	}
	
	AIController->StopThinking();
}
