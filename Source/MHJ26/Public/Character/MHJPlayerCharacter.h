// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "Character/MHJCharacter.h"
#include "MHJPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class MHJ26_API AMHJPlayerCharacter : public AMHJCharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	float ViewPitchMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Input")
	float ViewPitchMax;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> GeneralInputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> CinematicInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> RunAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> AttackAction;
	
private:
	uint8 bInCinematic:1;
	
public:
	AMHJPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void EnterCinematic();
	UFUNCTION(BlueprintCallable, Category="Input")
	void EnterGameplay();
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void StartRunning(const FInputActionValue& Value);
	UFUNCTION()
	void StopRunning(const FInputActionValue& Value);
	
};
