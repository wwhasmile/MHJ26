// Copyright (c) 2026 Hasmile


#include "Interaction/MHJInteractionComponent.h"

UMHJInteractionComponent::UMHJInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

bool UMHJInteractionComponent::IsFacingInteractable() const
{
	return true;
}

bool UMHJInteractionComponent::CanInteract(AActor* Instigator) const
{
	return true;
}

FText UMHJInteractionComponent::GetInteractionPrompt(AActor* Instigator) const
{
	return FText::GetEmpty();
}

bool UMHJInteractionComponent::Interact(AActor* Instigator)
{
	return true;
}


void UMHJInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UMHJInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

