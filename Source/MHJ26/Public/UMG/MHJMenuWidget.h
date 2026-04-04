// Copyright (c) 2026 Hasmile

#pragma once

#include "Blueprint/UserWidget.h"
#include "MHJMenuWidget.generated.h"

class USoundBase;

UENUM(BlueprintType)
enum class EMenuWidgetResponse : uint8
{
	MWR_None UMETA(DisplayName="None"),
	MWR_Success UMETA(DisplayName="Success"),
	MWR_Proceed UMETA(DisplayName="Proceed"),
	MWR_Cancelled UMETA(DisplayName="Cancelled"),
	MWR_Question UMETA(DisplayName="Question"),
	MWR_Failed UMETA(DisplayName="Failed"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMenuWidgetOpenedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMenuWidgetClosedSignature);

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sounds")
	TObjectPtr<USoundBase> SuccessSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sounds")
	TObjectPtr<USoundBase> ProceedSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sounds")
	TObjectPtr<USoundBase> CancelSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sounds")
	TObjectPtr<USoundBase> QuestionSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sounds")
	TObjectPtr<USoundBase> FailedSound;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FMenuWidgetOpenedSignature OnOpened;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FMenuWidgetClosedSignature OnClosed;

public:
	UFUNCTION(BlueprintCallable, Category="Widget")
	void Close();
	
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Input")
	EMenuWidgetResponse Accept();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Input")
	EMenuWidgetResponse Cancel();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Input")
	EMenuWidgetResponse Left();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Input")
	EMenuWidgetResponse Right();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Input")
	EMenuWidgetResponse Up();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Input")
	EMenuWidgetResponse Down();
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
};
