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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Display")
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Display")
	FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Display")
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	TMap<TSoftObjectPtr<UMHJItem>, TSoftObjectPtr<UMHJItem>> Combinations;
	
public:
	UMHJItem();
	
	FORCEINLINE UTexture2D* GetIcon() const { return Icon.Get(); }
	FORCEINLINE FText GetDisplayName() const { return DisplayName; }
	FORCEINLINE FText GetDescription() const { return Description; }
	
	UFUNCTION(BlueprintPure, Category="Item")
	FORCEINLINE bool IsCombinable() const { return !Combinations.IsEmpty(); }
	
	UFUNCTION(BlueprintCallable, Category="Item")
	bool Combine(const UMHJItem* With, UMHJItem*& OutItem) const;
	
	
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
};
