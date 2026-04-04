// Copyright (c) 2026 Hasmile


#include "Player/MHJPlayerController.h"

#include "UMG/MHJMenuWidget.h"

#include "EnhancedInputComponent.h"

void AMHJPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(PauseMenuWidgetClass))
	{
		InventoryWidget = CreateWidget<UMHJMenuWidget>(this, PauseMenuWidgetClass);
	}
	if (IsValid(InventoryWidgetClass))
	{
		InventoryWidget = CreateWidget<UMHJMenuWidget>(this, InventoryWidgetClass);
	}
}

void AMHJPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindKey(EKeys::Escape, EInputEvent::IE_Pressed, this, &AMHJPlayerController::OpenPauseMenu);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AMHJPlayerController::OpenInventory);
	}
}

void AMHJPlayerController::OpenPauseMenu()
{
	OpenMenuWidget(PauseMenuWidget);
}

void AMHJPlayerController::OpenInventory(const FInputActionValue& Value)
{
	OpenMenuWidget(InventoryWidget);
}

void AMHJPlayerController::OpenMenuWidget(UMHJMenuWidget* Widget)
{
	if (IsValid(ActiveMenuWidget))
	{
		return;
	}
	if (!IsValid(Widget))
	{
		return;
	}
	ActiveMenuWidget = Widget;
	ActiveMenuWidget->OnClosed.AddDynamic(this, &AMHJPlayerController::CloseMenuWidget);
	ActiveMenuWidget->AddToViewport();
	ActiveMenuWidget->SetFocus();
	SetPause(true);
	SetInputMode(FInputModeUIOnly());
}

void AMHJPlayerController::CloseMenuWidget()
{
	if (!IsValid(ActiveMenuWidget))
	{
		return;
	}
	ActiveMenuWidget->OnClosed.RemoveDynamic(this, &AMHJPlayerController::CloseMenuWidget);
	ActiveMenuWidget->RemoveFromParent();
	ActiveMenuWidget = nullptr;
	SetPause(false);
	SetInputMode(FInputModeGameOnly());
}
