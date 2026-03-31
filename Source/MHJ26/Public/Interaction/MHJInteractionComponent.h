// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MHJInteractionComponent.generated.h"

class IMHJInteractable;
class UPrimitiveComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MHJ26_API UMHJInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arm")
	float ArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arm")
	float ArmRadius;
	
private:
	TScriptInterface<IMHJInteractable> CurrentInteractable;
	
public:
	UMHJInteractionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintPure, Category="Interaction")
	AActor* GetCurrentInteractable() const;
	UFUNCTION(BlueprintPure, Category="Interaction")
	bool CanInteract() const;
	UFUNCTION(BlueprintPure, Category="Interaction")
	FText GetInteractionPrompt() const;
	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool Interact();

private:
	bool IsFacingInteractable() const;
	
};
