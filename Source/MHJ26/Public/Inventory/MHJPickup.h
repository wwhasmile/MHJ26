// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/MHJInteractable.h"
#include "MHJPickup.generated.h"

class UMHJItem;
class USoundBase;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMHJPickupTaken);

UCLASS()
class MHJ26_API AMHJPickup : public AActor, public IMHJInteractable
{
	GENERATED_BODY()

public:
	static FName CollisionComponentName;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TObjectPtr<UMHJItem> Item;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText PickedUpPrompt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText ItemExistsPrompt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TObjectPtr<USoundBase> PickedUpSound;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Item")
	FMHJPickupTaken OnTaken;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickupable", meta=(AllowPrivateAccess=true))
	TObjectPtr<USphereComponent> ColliderComponent;
	
public:
	AMHJPickup();
	
	virtual bool Interact_Implementation(AActor* Inst) override;
	virtual FText GetPrompt_Implementation(AActor* Inst) const override;
	
};
