// Copyright (c) 2026 HasmileHasmile


#include "AI/MHJBTTask_ClearBlackboardValue.h"

EBTNodeResult::Type UMHJBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}
	
	BB->ClearValue(Key.SelectedKeyName);
	return EBTNodeResult::Succeeded;
}
