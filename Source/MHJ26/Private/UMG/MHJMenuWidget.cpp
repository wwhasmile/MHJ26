// Copyright (c) 2026 Hasmile


#include "UMG/MHJMenuWidget.h"

void UMHJMenuWidget::Close()
{
	OnClosed.Broadcast();
}

void UMHJMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	OnOpened.Broadcast();
}

EMenuWidgetResponse UMHJMenuWidget::Accept_Implementation()
{
	return EMenuWidgetResponse::MWR_None;
}

EMenuWidgetResponse UMHJMenuWidget::Cancel_Implementation()
{
	return EMenuWidgetResponse::MWR_None;
}

EMenuWidgetResponse UMHJMenuWidget::Left_Implementation()
{
	return EMenuWidgetResponse::MWR_None;
}

EMenuWidgetResponse UMHJMenuWidget::Right_Implementation()
{
	return EMenuWidgetResponse::MWR_None;
}

EMenuWidgetResponse UMHJMenuWidget::Up_Implementation()
{
	return EMenuWidgetResponse::MWR_None;
}

EMenuWidgetResponse UMHJMenuWidget::Down_Implementation()
{
	return EMenuWidgetResponse::MWR_None;
}

FReply UMHJMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	EMenuWidgetResponse Result = EMenuWidgetResponse::MWR_None;
	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::SpaceBar)
	{
		Result = Accept();
	}
	else if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::BackSpace)
	{
		Result = Cancel();
	}
	else if (InKeyEvent.GetKey() == EKeys::Left || InKeyEvent.GetKey() == EKeys::A)
	{
		Result = Left();
	}
	else if (InKeyEvent.GetKey() == EKeys::Right || InKeyEvent.GetKey() == EKeys::D)
	{
		Result = Right();
	}
	else if (InKeyEvent.GetKey() == EKeys::Up || InKeyEvent.GetKey() == EKeys::W)
	{
		Result = Up();
	}
	else if (InKeyEvent.GetKey() == EKeys::Down || InKeyEvent.GetKey() == EKeys::S)
	{
		Result = Down();
	}
	
	if (Result == EMenuWidgetResponse::MWR_None)
	{
		return FReply::Unhandled();
	}
	
	FSlateSound SoundToPlay;
	switch (Result)
	{
	case EMenuWidgetResponse::MWR_Success:
		SoundToPlay.SetResourceObject(SuccessSound);
		break;
	case EMenuWidgetResponse::MWR_Proceed:
		SoundToPlay.SetResourceObject(ProceedSound);
		break;
	case EMenuWidgetResponse::MWR_Cancelled:
		SoundToPlay.SetResourceObject(CancelSound);
		break;
	case EMenuWidgetResponse::MWR_Question:
		SoundToPlay.SetResourceObject(QuestionSound);
		break;
	case EMenuWidgetResponse::MWR_Failed:
		SoundToPlay.SetResourceObject(FailedSound);
		break;
	}
	FSlateApplication::Get().PlaySound(SoundToPlay);
	
	return FReply::Handled();
}
