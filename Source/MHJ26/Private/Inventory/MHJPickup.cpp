// Copyright (c) 2026 Hasmile


#include "Inventory/MHJPickup.h"
#include "Inventory/MHJInventoryComponent.h"

#include "Components/SphereComponent.h"
#include "Inventory/MHJItem.h"
#include "Kismet/GameplayStatics.h"

const FName AMHJPickup::CollisionComponentName("CollisionComp");

AMHJPickup::AMHJPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ColliderComponent = CreateDefaultSubobject<USphereComponent>(CollisionComponentName);
	SetRootComponent(ColliderComponent);
}

int32 AMHJPickup::Interact_Implementation(AActor* Inst)
{
	if (!IsValid(Inst))
	{
		return -2;
	}
	
	UMHJInventoryComponent* InventoryComponent = Inst->FindComponentByClass<UMHJInventoryComponent>();
	if (!IsValid(InventoryComponent))
	{
		return -1;
	}
	
	if (!InventoryComponent->Add(Item.Get()))
	{
		return 0;
	}
	
	return 1;
}

void AMHJPickup::FinalizeInteraction_Implementation(AActor* Subject, int32 InteractionResult)
{
	if (InteractionResult)
	{
		if (IsValid(PickedUpSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), PickedUpSound);
		}
		OnTaken.Broadcast();
		Destroy();	
	}
}

FText AMHJPickup::GetMessage_Implementation(AActor* Subject, int32 InteractionResult) const
{
	if (InteractionResult < 0)
	{
		return FText::GetEmpty();
	}
	FFormatNamedArguments Arguments;
	Arguments.Add("ItemName", Item->GetDisplayName());
	return FText::Format(InteractionResult ? PickedUpPrompt : ItemExistsPrompt, Arguments);
}
