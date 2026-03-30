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
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	float ViewPitchMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	float ViewPitchMax;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> GeneralInputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> CinematicInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> RunAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> AttackAction;
	
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
