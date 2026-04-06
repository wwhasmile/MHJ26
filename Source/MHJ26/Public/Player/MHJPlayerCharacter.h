// Copyright (c) 2026 Hasmile

#pragma once

#include "ISpudObject.h"
#include "Character/MHJCharacter.h"
#include "MHJPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class UMHJInteractionComponent;
class UMHJInventoryComponent;

/**
 * 
 */
UCLASS()
class MHJ26_API AMHJPlayerCharacter : public AMHJCharacter, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()
	
public:
	static const FString PlayerCharacterName;
	
	static const FName FirstPersonCameraComponentName;
	static const FName FirstPersonInteractionComponentName;
	static const FName InventoryComponentName;
	
	static const FName FirstPersonCameraSocketName;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera", meta=(ClampMin=60, UIMin=60))
	float BaseFieldOfView;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera", meta=(ClampMin=0, UIMin=0))
	float RunningFieldOfViewModifier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera", meta=(ClampMin=0, UIMin=0))
	float RunningFieldOfViewInterpolationSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera", meta=(ClampMin=0, UIMin=0))
	float WalkingFieldOfViewInterpolationSpeed;
	
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
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> RunAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> AttackAction;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Interaction", meta=(AllowPrivateAccess=true))
	TObjectPtr<UMHJInteractionComponent> FirstPersonInteraction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess=true))
	TObjectPtr<UMHJInventoryComponent> Inventory;
	
	uint8 bInCinematic:1;
	
public:
	AMHJPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void EnterCinematic();
	UFUNCTION(BlueprintCallable, Category="Input")
	void EnterGameplay();
	
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCamera; }
	FORCEINLINE UMHJInteractionComponent* GetFirstPersonInteraction() const { return FirstPersonInteraction; }
	FORCEINLINE UMHJInventoryComponent* GetInventory() const { return Inventory; }
	
	virtual FString OverrideName_Implementation() const override;
	
	virtual void SpudStoreCustomData_Implementation(const USpudState* State, USpudStateCustomData* CustomData) override;
	virtual void SpudRestoreCustomData_Implementation(USpudState* State, USpudStateCustomData* CustomData) override;
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void StartCrouching(const FInputActionValue& Value);
	UFUNCTION()
	void StopCrouching(const FInputActionValue& Value);
	
	UFUNCTION()
	void StartRunning(const FInputActionValue& Value);
	UFUNCTION()
	void StopRunning(const FInputActionValue& Value);
	
	UFUNCTION()
	void Interact(const FInputActionValue& Value);
	
};
