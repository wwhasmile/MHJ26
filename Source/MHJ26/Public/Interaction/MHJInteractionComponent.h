// Copyright (c) 2026 Hasmile

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MHJInteractionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MHJ26_API UMHJInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arm")
	float ArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arm")
	float ArmRadius;
	
public:
	UMHJInteractionComponent();
	
	UFUNCTION(BlueprintPure, Category="Interaction")
	bool IsFacingInteractable() const;
	UFUNCTION(BlueprintPure, Category="Interaction")
	bool CanInteract(AActor* Instigator) const;
	UFUNCTION(BlueprintPure, Category="Interaction")
	FText GetInteractionPrompt(AActor* Instigator) const;
	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool Interact(AActor* Instigator);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
