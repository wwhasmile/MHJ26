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
	if (bHit && IsValid(HitActor) && HitActor->Implements<UMHJInteractable>())
	{
		CurrentInteractable = TScriptInterface<IMHJInteractable>(OutHit.GetActor());
	}
	else
	{
		CurrentInteractable = nullptr;
	}
	
	FLinearColor TraceColor = FLinearColor::Yellow;
	FLinearColor TraceHitColor = FLinearColor::Green;
	float DrawTime = PrimaryComponentTick.TickInterval;
	DrawDebugSphereTraceSingle(GetWorld(), Start, End, ArmRadius, EDrawDebugTrace::ForDuration, bHit, OutHit, TraceColor, TraceHitColor, DrawTime);
}

AActor* UMHJInteractionComponent::GetCurrentInteractable() const
{
	return IsFacingInteractable() ? Cast<AActor>(CurrentInteractable.GetObject()) : nullptr;
}

bool UMHJInteractionComponent::CanInteract() const
{
	return IsFacingInteractable() && IMHJInteractable::Execute_CanInteract(CurrentInteractable.GetObject(), GetOwner());
}

FText UMHJInteractionComponent::GetInteractionPrompt() const
{
	return CanInteract() ? IMHJInteractable::Execute_GetPrompt(CurrentInteractable.GetObject(), GetOwner()) : FText::GetEmpty();
}

bool UMHJInteractionComponent::Interact()
{
	return CanInteract() && IMHJInteractable::Execute_Interact(CurrentInteractable.GetObject(), GetOwner());
}

bool UMHJInteractionComponent::IsFacingInteractable() const
{
	return CurrentInteractable.GetObject() != nullptr;
}