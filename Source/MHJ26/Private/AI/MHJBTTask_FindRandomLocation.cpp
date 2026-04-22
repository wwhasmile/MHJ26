// Copyright (c) 2026 Hasmile


#include "AI/MHJBTTask_FindRandomLocation.h"

#include "NavigationSystem.h"

UMHJBTTask_FindRandomLocation::UMHJBTTask_FindRandomLocation()
{
	Radius = 15000.0f;
}

EBTNodeResult::Type UMHJBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}
	
	const FVector Origin = OwnerComp.GetOwner()->GetActorLocation();
	FNavLocation Result;
	if (NavSys->GetRandomPointInNavigableRadius(Origin, Radius.GetValue(BB), Result))
	{
		BB->SetValueAsVector(LocationKey.SelectedKeyName, Result.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
