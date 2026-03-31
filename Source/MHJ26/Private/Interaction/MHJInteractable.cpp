// Copyright (c) 2026 Hasmile


#include "Interaction/MHJInteractable.h"

bool IMHJInteractable::CanInteract_Implementation(AActor* Instigator) const
{
	return true;
}

FText IMHJInteractable::GetPrompt_Implementation(AActor* Instigator) const
{
	return FText::GetEmpty();
}

