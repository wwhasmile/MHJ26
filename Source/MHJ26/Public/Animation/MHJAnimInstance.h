// Copyright (c) 2026 Hasmile

#pragma once

#include "Animation/AnimInstance.h"
#include "MHJAnimInstance.generated.h"

class AMHJCharacter;
class UMHJCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess=true))
	TObjectPtr<APawn> PawnOwner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess=true))
	TObjectPtr<AMHJCharacter> CharacterOwner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess=true))
	TObjectPtr<UMHJCharacterMovementComponent> OwnerMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn|Input", meta=(AllowPrivateAccess=true))
	FRotator ControlRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn|Movement", meta=(AllowPrivateAccess=true))
	FRotator ActorRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn|Movement", meta=(AllowPrivateAccess=true))
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn|Movement", meta=(AllowPrivateAccess=true))
	float VelocityLength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn|Movement", meta=(AllowPrivateAccess=true))
	float HorizontalVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn|Movement", meta=(AllowPrivateAccess=true))
	float HorizontalActorDirection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn|Movement", meta=(AllowPrivateAccess=true))
	float HorizontalControlDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Movement", meta=(AllowPrivateAccess=true))
	bool bRunning;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Movement", meta=(AllowPrivateAccess=true))
	bool bCrouching;
	
public:
	FORCEINLINE AMHJCharacter* GetCharacterOwner() const { return CharacterOwner; }
	FORCEINLINE UMHJCharacterMovementComponent* GetOwnerMovementComponent() const { return OwnerMovementComponent; }
	
	FORCEINLINE FRotator GetControlRotation() const { return ControlRotation; }
	
	FORCEINLINE FRotator GetActorRotation() const { return ControlRotation; }
	
	FORCEINLINE FVector GetVelocity() const { return Velocity; }
	FORCEINLINE float GetVelocityLength() const { return VelocityLength; }
	FORCEINLINE float GetHorizontalVelocity() const { return HorizontalVelocity; }
	FORCEINLINE float GetHorizontalActorDirection() const { return HorizontalActorDirection; }
	FORCEINLINE float GetHorizontalControlDirection() const { return HorizontalControlDirection; }
	
	FORCEINLINE bool IsRunning() const { return bRunning; }
	FORCEINLINE bool IsCrouching() const { return bCrouching; }
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
};
