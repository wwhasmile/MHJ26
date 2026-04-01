// Copyright (c) 2026 Hasmile


#include "Inventory/MHJInventoryComponent.h"
#include "Inventory/MHJItem.h"


UMHJInventoryComponent::UMHJInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UMHJInventoryComponent::GetItem(int32 Index, UMHJItem*& OutItem) const
{
	if (Slots.IsValidIndex(Index) && Slots[Index].IsValid())
	{
		OutItem = Slots[Index].Get();
		return true;
	}
	OutItem = nullptr;
	return false;
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

bool UMHJInventoryComponent::Combine(UMHJItem* ItemA, UMHJItem* ItemB, UMHJItem*& OutItem)
{
	OutItem = nullptr;
	
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
	
	const int Index = ExistingItems[ItemB];
	Slots[Index] = OutItem;
	ExistingItems.Remove(ItemB);
	ExistingItems.Add(OutItem, Index);
	
	Remove(ItemA);
	return true;
}

void UMHJInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ExistingItems.Reserve(Slots.Num());
	TArray<TSoftObjectPtr<UMHJItem>> CleanedSlots;
	CleanedSlots.Reserve(Slots.Num());
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (!ExistingItems.Contains(Slots[i]))
		{
			const int index = CleanedSlots.Add(Slots[i]);
			ExistingItems.Add(Slots[i], index);
		}
	}
	Slots = CleanedSlots;
}
