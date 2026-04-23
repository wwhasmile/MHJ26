// Copyright (c) 2026 Hasmile


#include "AI/MHJAIController.h"
#include "AI/MHJAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Perception/AIPerceptionComponent.h"


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
	HearingConfig->SetMaxAge(5.0f);
}

void AMHJAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if ((ControlledCharacter = Cast<AMHJAICharacter>(InPawn)))
	{
		if (UBehaviorTree* BehaviorTree = ControlledCharacter->GetBehaviorTree())
		{
			UBlackboardComponent* BB;
			if (UseBlackboard(BehaviorTree->BlackboardAsset, BB))
			{
				RunBehaviorTree(BehaviorTree);
			}
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
		else
		{
			PerceptionComponent->SetDominantSense(nullptr);
		}
	}
	else
	{
		ControlledCharacter = nullptr;
	}
	PerceptionComponent->RequestStimuliListenerUpdate();
	
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMHJAIController::OnTargetPerceptionUpdated);
}

void AMHJAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	ControlledCharacter = nullptr;
	
	PerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AMHJAIController::OnTargetPerceptionUpdated);
}

void AMHJAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)
	{
		return;
	}
	
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		BB->ClearValue(NoiseLocationBlackboardKeyName);
		if (Stimulus.WasSuccessfullySensed())
		{
			World->GetTimerManager().ClearTimer(EnemyLostTimerHandle);
			BB->ClearValue(TargetLastSeenAtBlackboardKeyName);
			BB->SetValueAsObject(TargetBlackboardKeyName, Actor);
		}
		else
		{
			FTimerDelegate EnemyLostTimerDelegate;
			EnemyLostTimerDelegate.BindUObject(this, &AMHJAIController::OnEnemyLost, Actor);
			World->GetTimerManager().SetTimer(EnemyLostTimerHandle, EnemyLostTimerDelegate, SightConfig->GetMaxAge(), false);
		}
	}
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		BB->SetValueAsVector(NoiseLocationBlackboardKeyName, Stimulus.StimulusLocation);
	}
}

void AMHJAIController::OnEnemyLost(AActor* Actor)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB)
	{
		return;
	}
	
	if (Actor)
	{
		BB->SetValueAsVector(TargetLastSeenAtBlackboardKeyName, Actor->GetActorLocation());
	}
	else
	{
		BB->ClearValue(TargetLastSeenAtBlackboardKeyName);
	}
	
	BB->ClearValue(TargetBlackboardKeyName);
}
