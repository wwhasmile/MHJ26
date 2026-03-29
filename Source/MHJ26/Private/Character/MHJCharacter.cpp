// Copyright (c) 2026 Hasmile


#include "Character/MHJCharacter.h"

#include "Character/MHJCharacterMovementComponent.h"

AMHJCharacter::AMHJCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMHJCharacterMovementComponent>(CharacterMovementComponentName))
{
	MHJCharacterMovement = GetCharacterMovement<UMHJCharacterMovementComponent>();
	
}

