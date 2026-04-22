// Copyright (c) 2026 Hasmile

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "MHJBTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJBTTask_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Node")
	FBlackboardKeySelector LocationKey;
	
	UPROPERTY(EditAnywhere, Category = "Node", meta=(UIMin = 0, ClampMin = 0))
	FValueOrBBKey_Float Radius;
	
public:
	UMHJBTTask_FindRandomLocation();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
