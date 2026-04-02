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
	bool CanInteract(AActor* Subject) const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	int32 Interact(AActor* Subject);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	void FinalizeInteraction(AActor* Subject, int32 InteractionResult);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	FText GetMessage(AActor* Subject, int32 InteractionResult) const;
	
	virtual bool CanInteract_Implementation(AActor* Subject) const;
	virtual FText GetMessage_Implementation(AActor* Subject, int32 InteractionResult) const;
};
