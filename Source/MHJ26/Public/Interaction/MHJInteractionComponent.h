// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MHJInteractionComponent.generated.h"

class IMHJInteractable;
class UPrimitiveComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionComponentInteracted, TScriptInterface<IMHJInteractable>, Interactable, int32, InteractionResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionComponentHover);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionComponentUnhover);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MHJ26_API UMHJInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arm")
	float ArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arm")
	float ArmRadius;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Interaction")
	FOnInteractionComponentInteracted OnInteracted;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Interaction")
	FOnInteractionComponentHover OnHover;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Interaction")
	FOnInteractionComponentUnhover OnUnhover;
	
private:
	TScriptInterface<IMHJInteractable> CurrentInteractable;
	
public:
	UMHJInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintPure, Category="Interaction")
	AActor* GetCurrentInteractable() const;
	UFUNCTION(BlueprintPure, Category="Interaction")
	FText GetMessage(int32 InteractionResult) const;
	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool Interact();

private:
	bool IsFacingInteractable() const;
	
};
