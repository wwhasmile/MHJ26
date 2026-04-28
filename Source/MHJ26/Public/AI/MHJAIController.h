// Copyright (c) 2026 Hasmile

#pragma once

#include "Perception/AIPerceptionTypes.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "MHJAIController.generated.h"

class UAIPerceptionComponent;
class UBehaviorTree;
class AMHJAICharacter;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS(Abstract)
class MHJ26_API AMHJAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	static FName AIPerceptionComponentName;
	static FName SightConfigName;
	static FName HearingConfigName;
	
	static FName StateBlackboardKeyName;
	static FName TargetBlackboardKeyName;
	static FName TargetLastSeenAtBlackboardKeyName;
	static FName NoiseLocationBlackboardKeyName;
	static FName MoveLocationBlackboardKeyName;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Controller", meta = (AllowPrivateAccess = true))
	TObjectPtr<AMHJAICharacter> ControlledCharacter;
	
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	
	FTimerHandle EnemyLostTimerHandle;
	
public:
	AMHJAIController();
	
	FORCEINLINE AMHJAICharacter* GetControlledCharacter() const { return ControlledCharacter; }
	
	void StartThinking();
	void StopThinking();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
private:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UFUNCTION()
	void OnEnemyLost(AActor* Actor);
	
};
