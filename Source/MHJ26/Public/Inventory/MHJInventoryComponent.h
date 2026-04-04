// Copyright (c) 2026 Hasmile

#pragma once

#include "MHJInventoryComponent.generated.h"

class UMHJItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MHJ26_API UMHJInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	TMap<TObjectPtr<UMHJItem>, int32> ExistingItems;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UMHJItem>> Slots;
	
public:
	UMHJInventoryComponent();
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	FORCEINLINE int32 GetNumberOfSlots() const { return Slots.Num(); }
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool GetItem(int32 Index, UMHJItem*& OutItem) const;
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool Contains(UMHJItem* Item) const;
	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 IndexOf(UMHJItem* Item) const;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Add(UMHJItem* Item);
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void Remove(UMHJItem* Item);
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Combine(UMHJItem* ItemA, UMHJItem* ItemB, UMHJItem*& OutItem, int32& OutIndex);
	
};
