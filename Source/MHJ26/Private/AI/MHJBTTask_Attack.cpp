// Copyright (c) 2026 Hasmile


#include "AI/MHJBTTask_Attack.h"

#include "AIController.h"
#include "Engine/DamageEvents.h"

UMHJBTTask_Attack::UMHJBTTask_Attack()
{
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UMHJBTTask_Attack, TargetKey), AActor::StaticClass());
	DamageType.SetBaseClass(UDamageType::StaticClass());
}

EBTNodeResult::Type UMHJBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}
	
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}
	
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* Pawn = Controller->GetPawn();
	
	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = DamageType.GetValue<UDamageType>(OwnerComp);
	Target->TakeDamage(DamageAmount.GetValue(OwnerComp), DamageEvent, Controller, Pawn);
	return EBTNodeResult::Succeeded;
}
