// Copyright (c) 2026 Hasmile


#include "Inventory/MHJItem.h"

bool UMHJItem::Combine(const UMHJItem* With, UMHJItem*& OutItem) const
{
	OutItem = nullptr;
	
	if (!Combinations.Contains(With))
	{
		return false;
	}
	
	OutItem = Combinations[With].LoadSynchronous();
	return true;
}

FPrimaryAssetId UMHJItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Item", GetFName());
}
