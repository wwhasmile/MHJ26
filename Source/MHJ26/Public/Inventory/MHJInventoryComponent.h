// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"

#include "MHJInventoryComponent.generated.h"

class UMHJItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MHJ26_API UMHJInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	TMap<TSoftObjectPtr<UMHJItem>, int32> ExistingItems;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TArray<TSoftObjectPtr<UMHJItem>> Slots;
	
public:
	UMHJInventoryComponent();
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	FORCEINLINE int32 GetNumberOfSlots() const { return Slots.Num(); }
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool GetItem(int32 Index, UMHJItem*& OutItem) const;
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	FORCEINLINE bool Contains(UMHJItem* Item) const { return ExistingItems.Contains(Item); }
	UFUNCTION(BlueprintPure, Category="Inventory")
	FORCEINLINE int32 IndexOf(UMHJItem* Item) const { return ExistingItems.Contains(Item) ? ExistingItems[Item] : -1; }
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Add(UMHJItem* Item);
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void Remove(UMHJItem* Item);
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool Combine(UMHJItem* ItemA, UMHJItem* ItemB, UMHJItem*& OutItem);

protected:
	virtual void BeginPlay() override;
	
};
