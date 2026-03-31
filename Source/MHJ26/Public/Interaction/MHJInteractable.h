// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MHJInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UMHJInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MHJ26_API IMHJInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	bool CanInteract(AActor* Instigator) const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	bool Interact(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	FText GetPrompt(AActor* Instigator) const;
	
	virtual bool CanInteract_Implementation(AActor* Instigator) const;
	virtual FText GetPrompt_Implementation(AActor* Instigator) const;
};
