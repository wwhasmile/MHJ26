// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MHJBTTask_StopRunning.generated.h"

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJBTTask_StopRunning : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
