// Copyright (c) 2026 Hasmile


#include "Inventory/MHJInventoryComponent.h"
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
