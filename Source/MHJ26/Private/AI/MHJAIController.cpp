// Copyright (c) 2026 Hasmile


#include "AI/MHJAIController.h"
#include "AI/MHJAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"

#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"


FName AMHJAIController::AIPerceptionComponentName("AIPerceptionComp");
FName AMHJAIController::SightConfigName("SightConf");
FName AMHJAIController::HearingConfigName("HearingConf");

FName AMHJAIController::StateBlackboardKeyName("State");
FName AMHJAIController::TargetBlackboardKeyName("Target");
FName AMHJAIController::TargetLastSeenAtBlackboardKeyName("TargetLastSeen");
FName AMHJAIController::NoiseLocationBlackboardKeyName("NoiseLocation");
FName AMHJAIController::MoveLocationBlackboardKeyName("MoveLocation");

AMHJAIController::AMHJAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(AIPerceptionComponentName);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(SightConfigName);
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(HearingConfigName);
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;

	AAIController::SetGenericTeamId(FGenericTeamId(2));
}

void AMHJAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if ((ControlledCharacter = Cast<AMHJAICharacter>(InPawn)))
	{
		if (UBehaviorTree* BehaviorTree = ControlledCharacter->GetBehaviorTree())
		{
			UBlackboardComponent* BB = GetBlackboardComponent();
			UseBlackboard(BehaviorTree->BlackboardAsset, BB);
			RunBehaviorTree(BehaviorTree);
		}
		
		if (ControlledCharacter->bEnableSight)
		{
			SightConfig->SightRadius = ControlledCharacter->SightRadius;
			SightConfig->LoseSightRadius = ControlledCharacter->SightLossRadius;
			SightConfig->PeripheralVisionAngleDegrees = ControlledCharacter->PeripheralRange;
			SightConfig->SetMaxAge(ControlledCharacter->SightMemory);
		}
		else
		{
			SightConfig->SightRadius = 0.0f;
			SightConfig->LoseSightRadius = 0.0f;
			SightConfig->PeripheralVisionAngleDegrees = 0.0f;
			SightConfig->SetMaxAge(0.0f);
		}
		PerceptionComponent->ConfigureSense(*SightConfig);
		
		if (ControlledCharacter->bEnableHearing)
		{
			HearingConfig->HearingRange = ControlledCharacter->HearingRange;
		}
		else
		{
			HearingConfig->HearingRange = 0.0f;
		}
		PerceptionComponent->ConfigureSense(*HearingConfig);
		
		if (ControlledCharacter->bEnableSight)
		{
			PerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
		}
		else if (ControlledCharacter->bEnableHearing)
		{
			PerceptionComponent->SetDominantSense(UAISense_Hearing::StaticClass());
		}
	}
	else
	{
		ControlledCharacter = nullptr;
	}
	
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMHJAIController::OnTargetPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AMHJAIController::OnTargetPerceptionForgotten);
}

void AMHJAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	ControlledCharacter = nullptr;
	
	PerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AMHJAIController::OnTargetPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionForgotten.RemoveDynamic(this, &AMHJAIController::OnTargetPerceptionForgotten);
}

void AMHJAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)
	{
		return;
	}
	
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BB->ClearValue(TargetLastSeenAtBlackboardKeyName);
			BB->SetValueAsObject(TargetBlackboardKeyName, Actor);
		}
		else
		{
			Blackboard->ClearValue(TargetBlackboardKeyName);
			Blackboard->SetValueAsVector(TargetLastSeenAtBlackboardKeyName, Stimulus.StimulusLocation);
		}
		return;
	}
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BB->SetValueAsVector(NoiseLocationBlackboardKeyName, Stimulus.StimulusLocation);
		}
	}
}

void AMHJAIController::OnTargetPerceptionForgotten(AActor* Actor)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)
	{
		return;
	}
	
	BB->ClearValue(TargetLastSeenAtBlackboardKeyName);
	BB->ClearValue(TargetBlackboardKeyName);
}
