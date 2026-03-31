// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"

#include "MHJItem.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin=1, UIMin=1))
	uint8 MaxStackSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSoftObjectPtr<UMHJItem>, TSoftObjectPtr<UMHJItem>> Combinations;
	
public:
	UMHJItem();
	
	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE UTexture2D* GetIcon() const { return Icon.Get(); }
	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE FText GetDisplayName() const { return DisplayName; }
	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE FText GetDescription() const { return Description; }
	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE bool IsStackable() const { return MaxStackSize > 1; }
	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE uint8 GetMaxStackSize() const { return MaxStackSize; }
	
	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE bool IsCombinable() const { return !Combinations.IsEmpty(); }
	
	UFUNCTION(BlueprintCallable, Category="Item")
	bool Combine(const UMHJItem* With, UMHJItem*& OutItem) const;
	
};
