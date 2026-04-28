// Copyright (c) 2026 Hasmile

#pragma once

#include "ISpudObject.h"
#include "Character/MHJCharacter.h"
#include "MHJAICharacter.generated.h"

class UBehaviorTree;

UCLASS(Abstract)
class MHJ26_API AMHJAICharacter : public AMHJCharacter, public ISpudObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bAutoStartThinking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Sight")
	bool bEnableSight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Sight", meta=(ClampMin=0, UIMin=0, ForceUnits="Centimeters", EditCondition="bEnableSight"))
	float SightRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Sight", meta=(ClampMin=0, UIMin=0, ForceUnits="Centimeters", EditCondition="bEnableSight"))
	float SightLossRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Sight", meta=(ClampMin=0, UIMin=0, ClampMax=90, UIMax=90, ForceUnits="Degrees", EditCondition="bEnableSight"))
	float PeripheralRange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Sight", meta=(ClampMin=0, UIMin=0, EditCondition="bEnableSight"))
	float SightMemory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Hearing")
	bool bEnableHearing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Hearing", meta=(ClampMin=0, UIMin=0, ForceUnits="Centimeters", EditCondition="bEnableHearing"))
	float HearingRange;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta=(AllowPrivateAccess=true))
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	FGuid SpudGuid;
	
public:
	AMHJAICharacter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StartThinking();
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StopThinking();
	
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	
};
