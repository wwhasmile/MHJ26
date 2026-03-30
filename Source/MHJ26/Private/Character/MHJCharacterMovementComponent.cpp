// Copyright (c) 2026 Hasmile


#include "Character/MHJCharacterMovementComponent.h"

UMHJCharacterMovementComponent::UMHJCharacterMovementComponent()
{
	MaxWalkSpeed = 230.0f;
	MaxRunSpeed = 450.0f;
}

void UMHJCharacterMovementComponent::StartRunning()
{
	bRunning = true;
}

void UMHJCharacterMovementComponent::StopRunning()
{
	bRunning = false;
}

float UMHJCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking && !IsCrouching() && IsRunning())
	{
		return MaxRunSpeed;
	}
	return Super::GetMaxSpeed();
}
