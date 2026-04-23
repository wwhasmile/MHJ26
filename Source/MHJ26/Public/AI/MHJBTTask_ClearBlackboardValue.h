// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "MHJBTTask_ClearBlackboardValue.generated.h"

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJBTTask_ClearBlackboardValue : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Node")
	FBlackboardKeySelector Key;
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
