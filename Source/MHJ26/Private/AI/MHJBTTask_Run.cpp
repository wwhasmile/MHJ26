// Copyright (c) 2026 Hasmile


#include "AI/MHJBTTask_Run.h"

#include "AIController.h"
#include "Character/MHJCharacter.h"
#include "Character/MHJCharacterMovementComponent.h"

EBTNodeResult::Type UMHJBTTask_Run::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}
	
	if (AMHJCharacter* Character = Cast<AMHJCharacter>(Pawn))
	{
		UMHJCharacterMovementComponent* Movement = Character->GetCharacterMovement<UMHJCharacterMovementComponent>();
		if (!Movement)
		{
			return EBTNodeResult::Failed;
		}
		Movement->StartRunning();
	}
	return EBTNodeResult::Succeeded;
}
