// Copyright (c) 2026 Hasmile


#include "Player/MHJPlayerHUD.h"

#include "Blueprint/UserWidget.h"

AMHJPlayerHUD::AMHJPlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMHJPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(WidgetClass))
	{
		WidgetInstance = CreateWidget<UUserWidget>(GetOwningPlayerController(), WidgetClass);
		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport();
		}
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
