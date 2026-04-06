// Copyright (c) 2026 Hasmile

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "MHJCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Movement: Walking")
	uint8 bCanRun:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Movement: Walking", meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MaxRunSpeed;
	
private:
	uint8 bRunning:1;
	
public:
	UMHJCharacterMovementComponent();
	
	UFUNCTION(BlueprintCallable, Category="Character Movement: Walking")
	void StartRunning();
	UFUNCTION(BlueprintCallable, Category="Character Movement: Walking")
	void StopRunning();
	
	UFUNCTION(BlueprintPure, Category="Character Movement: Walking")
	FORCEINLINE bool IsRunning() const { return bCanRun && bRunning; }
	
	virtual float GetMaxSpeed() const override;
	
};
