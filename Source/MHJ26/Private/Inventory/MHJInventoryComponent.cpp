// Copyright (c) 2026 Hasmile


#include "Inventory/MHJInventoryComponent.h"

#include "Engine/AssetManager.h"
#include "Inventory/MHJItem.h"


UMHJInventoryComponent::UMHJInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UMHJInventoryComponent::GetItem(int32 Index, UMHJItem*& OutItem) const
{
	if (Slots.IsValidIndex(Index) && IsValid(Slots[Index]))
	{
		OutItem = Slots[Index];
		return true;
	}
	OutItem = nullptr;
	return false;
}

bool UMHJInventoryComponent::Contains(UMHJItem* Item) const
{
	if (!IsValid(Item))
	{
		return false;
	}
	return ExistingItems.Contains(Item);
}

int32 UMHJInventoryComponent::IndexOf(UMHJItem* Item) const
{
	if (!IsValid(Item))
	{
		return -1;
	}
	if (!ExistingItems.Contains(Item))
	{
		return -1;
	}
	return ExistingItems[Item];
}

bool UMHJInventoryComponent::Add(UMHJItem* Item)
{
	if (!IsValid(Item))
	{
		return false;
	}
	if (ExistingItems.Contains(Item))
	{
		return false;
	}
	const int index = Slots.Add(Item);
	ExistingItems.Add(Item, index);
	return true;
}

void UMHJInventoryComponent::Remove(UMHJItem* Item)
{
	if (!IsValid(Item))
	{
		return;
	}
	if (!ExistingItems.Contains(Item))
	{
		return;
	}
	Slots.RemoveAt(ExistingItems[Item]);
	for (int32 i = ExistingItems[Item]; i < Slots.Num(); ++i)
	{
		ExistingItems[Slots[i]] = i;
	}
	ExistingItems.Remove(Item);
}

bool UMHJInventoryComponent::Combine(UMHJItem* ItemA, UMHJItem* ItemB, UMHJItem*& OutItem, int32& OutIndex)
{
	OutItem = nullptr;
	OutIndex = -1;
	
	if (ItemA == ItemB)
	{
		return false;
	}
	if (!IsValid(ItemA) || !IsValid(ItemB))
	{
		return false;
	}
	
	if (!ExistingItems.Contains(ItemA) || !ExistingItems.Contains(ItemB))
	{
		return false;
	}
	
	if (!ItemA->Combine(ItemB, OutItem))
	{
		return false;
	}
	
	if (ExistingItems.Contains(OutItem))
	{
		OutItem = nullptr;
		return false;
	}
	
	Remove(ItemA);
	
	OutIndex = ExistingItems[ItemB];
	Slots[OutIndex] = OutItem;
	ExistingItems.Remove(ItemB);
	ExistingItems.Add(OutItem, OutIndex);
	
	return true;
}

TArray<FPrimaryAssetId> UMHJInventoryComponent::GetSlotsSerialized() const
{
	TArray<FPrimaryAssetId> Result;
	Result.Reserve(Slots.Num());
	
	for (const UMHJItem* Item : Slots)
	{
		Result.Add(Item->GetPrimaryAssetId());
	}
	return Result;
}

void UMHJInventoryComponent::LoadSlotsSerialized(const TArray<FPrimaryAssetId>& Items)
{
	Slots.Empty(Items.Num());
	ExistingItems.Empty();
	
	UAssetManager& AssetManager = UAssetManager::Get();
	TSharedPtr<FStreamableHandle> Handle = AssetManager.LoadPrimaryAssets(Items);
	if (Handle.IsValid())
	{
		Handle->WaitUntilComplete();
	}
	
	for (const FPrimaryAssetId& ItemId : Items)
	{
		if (UMHJItem* Item = AssetManager.GetPrimaryAssetObject<UMHJItem>(ItemId))
		{
			Slots.Add(Item);
			ExistingItems.Add(Item);
		}
	}
}
