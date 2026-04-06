// Copyright (c) 2026 Hasmile


#include "Animation/MHJAnimInstance.h"

#include "Character/MHJCharacter.h"
#include "Character/MHJCharacterMovementComponent.h"

#include "KismetAnimationLibrary.h"

void UMHJAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	PawnOwner = TryGetPawnOwner();
	if (!IsValid(PawnOwner))
	{
		return;
	}
	
	CharacterOwner = Cast<AMHJCharacter>(PawnOwner);
	if (!IsValid(CharacterOwner))
	{
		return;
	}
	OwnerMovementComponent = CharacterOwner->GetCharacterMovement<UMHJCharacterMovementComponent>();
}

void UMHJAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!IsValid(PawnOwner))
	{
		return;
	}
	
	ControlRotation = PawnOwner->GetControlRotation();
	ActorRotation = PawnOwner->GetActorRotation();
	Velocity = PawnOwner->GetVelocity();
	VelocityLength = Velocity.Length();
	FVector VelocityXY = FVector(Velocity.X, Velocity.Y, 0.0f);
	HorizontalVelocity = Velocity.Size2D();
	HorizontalActorDirection = UKismetAnimationLibrary::CalculateDirection(VelocityXY, ActorRotation);
	HorizontalControlDirection = UKismetAnimationLibrary::CalculateDirection(VelocityXY, ControlRotation);
	
	if (IsValid(CharacterOwner))
	{
		if (IsValid(OwnerMovementComponent))
		{
			bRunning = OwnerMovementComponent->IsRunning();
		}
		bCrouching = CharacterOwner->IsCrouched();
	}
}
