// Copyright (c) 2026 Hasmile


#include "Interaction/MHJInteractionComponent.h"
#include "Interaction/MHJInteractable.h"

#include "KismetTraceUtils.h"

UMHJInteractionComponent::UMHJInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void UMHJInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * ArmLength;
	FCollisionShape Shape = FCollisionShape::MakeSphere(ArmRadius);
	FHitResult OutHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	bool bHit = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Visibility, Shape, QueryParams);
	AActor* HitActor = OutHit.GetActor();
	if (bHit && IsValid(HitActor) && HitActor->Implements<UMHJInteractable>() && IMHJInteractable::Execute_CanInteract(HitActor, GetOwner()))
	{
		bFacingInteractable = true;
		if (CurrentInteractable != HitActor)
		{
			OnHover.Broadcast();
			CurrentInteractable = TScriptInterface<IMHJInteractable>(OutHit.GetActor());
		}
	}
	else if (bFacingInteractable)
	{
		bFacingInteractable = false;
		OnUnhover.Broadcast();
		CurrentInteractable = nullptr;
	}
	
	FLinearColor TraceColor = FLinearColor::Yellow;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = PrimaryComponentTick.TickInterval;
	DrawDebugSphereTraceSingle(GetWorld(), Start, End, ArmRadius, EDrawDebugTrace::ForDuration, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
}

AActor* UMHJInteractionComponent::GetCurrentInteractable() const
{
	return bFacingInteractable ? Cast<AActor>(CurrentInteractable.GetObject()) : nullptr;
}

FText UMHJInteractionComponent::GetMessage(int32 InteractionResult) const
{
	return bFacingInteractable ? IMHJInteractable::Execute_GetMessage(CurrentInteractable.GetObject(), GetOwner(), InteractionResult) : FText::GetEmpty();
}

bool UMHJInteractionComponent::Interact()
{
	if (!bFacingInteractable)
	{
		return false;
	}
	const int32 Result = IMHJInteractable::Execute_Interact(CurrentInteractable.GetObject(), GetOwner());
	OnInteracted.Broadcast(CurrentInteractable, Result);
	IMHJInteractable::Execute_FinalizeInteraction(CurrentInteractable.GetObject(), GetOwner(), Result);
	return true;
}