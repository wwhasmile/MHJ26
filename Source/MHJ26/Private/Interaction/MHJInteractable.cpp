// Copyright (c) 2026 Hasmile


#include "Interaction/MHJInteractable.h"

bool IMHJInteractable::CanInteract_Implementation(AActor* Subject) const
{
	return true;
}

FText IMHJInteractable::GetMessage_Implementation(AActor* Subject, int32 InteractionResult) const
{
	return FText::GetEmpty();
}

