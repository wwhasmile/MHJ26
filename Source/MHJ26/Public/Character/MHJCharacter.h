// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MHJCharacter.generated.h"

class UMHJCharacterMovementComponent;

UCLASS(Abstract)
class MHJ26_API AMHJCharacter : public ACharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement", meta=(AllowPrivateAccess=true))
	TObjectPtr<UMHJCharacterMovementComponent> MHJCharacterMovement;

public:
	AMHJCharacter(const FObjectInitializer& ObjectInitializer);

};
