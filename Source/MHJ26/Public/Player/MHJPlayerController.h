// Copyright (c) 2026 Hasmile

#pragma once

#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "MHJPlayerController.generated.h"

class UInputAction;
class UMHJMenuWidget;

/**
 * 
 */
UCLASS()
class MHJ26_API AMHJPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widgets")
	TSubclassOf<UMHJMenuWidget> PauseMenuWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widgets")
	TSubclassOf<UMHJMenuWidget> InventoryWidgetClass;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMHJMenuWidget> PauseMenuWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMHJMenuWidget> InventoryWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Widgets", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UMHJMenuWidget> ActiveMenuWidget;
	
	FGenericTeamId Team;
	
public:
	AMHJPlayerController();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
private:
	UFUNCTION()
	void OpenPauseMenu();
	UFUNCTION()
	void OpenInventory(const FInputActionValue& Value);
	
	void OpenMenuWidget(UMHJMenuWidget* Widget);
	UFUNCTION()
	void CloseMenuWidget();
	
};
