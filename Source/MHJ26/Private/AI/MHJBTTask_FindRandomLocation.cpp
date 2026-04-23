// Copyright (c) 2026 Hasmile


#include "AI/MHJBTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"

UMHJBTTask_FindRandomLocation::UMHJBTTask_FindRandomLocation()
{
	Radius = 15000.0f;
	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UMHJBTTask_FindRandomLocation, LocationKey));
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
	
	AActor* OwnerActor = OwnerComp.GetOwner();
	if (AAIController* Controller = OwnerComp.GetAIOwner())
	{
		OwnerActor = Controller->GetPawn();
	}
	if (!OwnerActor)
	{
		return EBTNodeResult::Failed;
	}
	const FVector Origin = OwnerActor->GetActorLocation();
	
	FNavLocation Result;
	if (!NavSys->GetRandomPointInNavigableRadius(Origin, Radius.GetValue(BB), Result))
	{
		return EBTNodeResult::Failed;
	}
	
	BB->SetValueAsVector(LocationKey.SelectedKeyName, Result.Location);
	return EBTNodeResult::Succeeded;
}
