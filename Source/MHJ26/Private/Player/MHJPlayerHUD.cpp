// Copyright (c) 2026 Hasmile


#include "Player/MHJPlayerHUD.h"

#include "Blueprint/UserWidget.h"

AMHJPlayerHUD::AMHJPlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMHJPlayerHUD::EnableHUD()
{
	if (!WidgetInstance)
	{
		return;
	}

	if (WidgetInstance->IsInViewport())
	{
		return;
	}
	
	WidgetInstance->AddToViewport();
}

void AMHJPlayerHUD::DisableHUD()
{
	if (!WidgetInstance)
	{
		return;
	}

	if (!WidgetInstance->IsInViewport())
	{
		return;
	}
	
	WidgetInstance->RemoveFromParent();
}

void AMHJPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(WidgetClass))
	{
		WidgetInstance = CreateWidget<UUserWidget>(GetOwningPlayerController(), WidgetClass);
		EnableHUD();
	}
}

void AMHJPlayerHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (IsValid(WidgetInstance))
	{
		WidgetInstance->RemoveFromParent();
		WidgetInstance = nullptr;
	}
}
