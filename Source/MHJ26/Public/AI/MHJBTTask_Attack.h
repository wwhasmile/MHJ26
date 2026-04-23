// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "MHJBTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Node")
	FBlackboardKeySelector TargetKey;
	
	UPROPERTY(EditAnywhere, Category = "Node", meta=(UIMin = 0, ClampMin = 0))
	FValueOrBBKey_Float DamageAmount;
	UPROPERTY(EditAnywhere, Category = "Node")
	FValueOrBBKey_Class DamageType;
	
public:
	UMHJBTTask_Attack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
