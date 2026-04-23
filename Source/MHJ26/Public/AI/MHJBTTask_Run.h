// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MHJBTTask_Run.generated.h"

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJBTTask_Run : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
