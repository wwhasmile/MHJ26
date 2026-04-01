// Copyright (c) 2026 Hasmile


#include "Inventory/MHJItem.h"

UMHJItem::UMHJItem()
{
}

bool UMHJItem::Combine(const UMHJItem* With, UMHJItem*& OutItem) const
{
	OutItem = nullptr;
	
	if (!Combinations.Contains(With))
	{
		return false;
	}
	
	OutItem = Combinations[With].Get();
	return true;
}