// Copyright (c) 2026 Hasmile


#include "Inventory/MHJPickup.h"
#include "Inventory/MHJInventoryComponent.h"

#include "Character/MHJPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

FName AMHJPickup::CollisionComponentName = FName(TEXT("CollisionComp"));

AMHJPickup::AMHJPickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ColliderComponent = CreateDefaultSubobject<USphereComponent>(CollisionComponentName);
	SetRootComponent(ColliderComponent);
}

bool AMHJPickup::Interact_Implementation(AActor* Inst)
{
	AMHJPlayerCharacter* PlayerCharacter = Cast<AMHJPlayerCharacter>(Inst);
	if (PlayerCharacter == nullptr)
	{
		return false;
	}
	
	UMHJInventoryComponent* InventoryComponent = PlayerCharacter->GetInventory();
	if (!IsValid(InventoryComponent))
	{
		return false;
	}
	
	if (!InventoryComponent->Add(Item.Get()))
	{
		return false;
	}
	
	if (IsValid(PickedUpSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), PickedUpSound);
	}
	Destroy();
	return false;
}

FText AMHJPickup::GetPrompt_Implementation(AActor* Inst) const
{
	return IMHJInteractable::GetPrompt_Implementation(Inst);
}
