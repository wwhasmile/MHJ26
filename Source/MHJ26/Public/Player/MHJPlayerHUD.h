// Copyright (c) 2026 Hasmile

#pragma once

#include "GameFramework/HUD.h"
#include "MHJPlayerHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS(Abstract)
class MHJ26_API AMHJPlayerHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HUD", meta=(AllowPrivateAccess=true))
	TSubclassOf<UUserWidget> WidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance;
	
public:
	AMHJPlayerHUD();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
