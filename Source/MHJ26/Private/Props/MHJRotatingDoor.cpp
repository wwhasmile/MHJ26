// Copyright (c) 2026 Hasmile


#include "Props/MHJRotatingDoor.h"

#include "AIController.h"
#include "NavLinkCustomComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TimelineComponent.h"
#include "Inventory/MHJInventoryComponent.h"
#include "Inventory/MHJItem.h"
#include "Kismet/GameplayStatics.h"
#include "NavAreas/NavArea_Null.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Navigation/PathFollowingComponent.h"


FName AMHJRotatingDoor::RootPivotComponentName("RootPivotComp");
FName AMHJRotatingDoor::PivotComponentName("PivotComp");
FName AMHJRotatingDoor::DoorMeshComponentName("DoorMeshComp");
FName AMHJRotatingDoor::DoorTimelineComponentName("DoorTimelineComp");
FName AMHJRotatingDoor::SmartLinkComponentName("SmartLinkComp");

FName AMHJRotatingDoor::DoorTimelineAlphaTrackName("Alpha");

#if WITH_EDITORONLY_DATA
FName AMHJRotatingDoor::DirectionArrowComponentName("DirectionArrowComp");
#endif

AMHJRotatingDoor::AMHJRotatingDoor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	State = EMHJRotatingDoorState::RDS_Closed;
	
	RemoveKey = true;
	
	OpenedRotation = FRotator(0.0f, 90.0f, 0.0f);
	ClosedRotation = FRotator(0.0f, 0.0f, 0.0f);
	
	OpenDuration = 0.5f;
	CloseDuration = 0.3f;
	
	RootPivot = CreateDefaultSubobject<USceneComponent>(RootPivotComponentName);
	SetRootComponent(RootPivot);
	
	Pivot = CreateDefaultSubobject<USceneComponent>(PivotComponentName);
	Pivot->SetupAttachment(RootPivot);
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(DoorMeshComponentName);
	DoorMesh->SetupAttachment(Pivot);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DoorMesh->SetCanEverAffectNavigation(false);
	
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(DoorTimelineComponentName);
	
	SmartLink = CreateDefaultSubobject<UNavLinkCustomComponent>(SmartLinkComponentName);
	SmartLink->SetNavigationRelevancy(true);
	SmartLink->SetMoveReachedLink(this, &AMHJRotatingDoor::NotifySmartLinkReached);
	SmartLink->SetLinkData(FVector(-100.0f, 0.0f, 0.0f), FVector(100.0f, 0.0f, 0.0f), ENavLinkDirection::BothWays);
	SmartLink->SetEnabledArea(UNavArea_Obstacle::StaticClass());
	SmartLink->SetDisabledArea(UNavArea_Null::StaticClass());
	
#if WITH_EDITORONLY_DATA
	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(DirectionArrowComponentName);
	DirectionArrow->SetupAttachment(GetRootComponent());
	DirectionArrow->SetArrowSize(5.0f);
	DirectionArrow->SetArrowLength(10.0f);
#endif
}

void AMHJRotatingDoor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (Curve)
	{
		FOnTimelineFloat OnTimelineFloat;
		OnTimelineFloat.BindDynamic(this, &AMHJRotatingDoor::TimelineFloat);
		DoorTimeline->AddInterpFloat(Curve, OnTimelineFloat, NAME_None, DoorTimelineAlphaTrackName);
		FOnTimelineEvent OnTimelineFinished;
		OnTimelineFinished.BindDynamic(this, &AMHJRotatingDoor::TimelineFinished);
		DoorTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
	}
	
	switch (State)
	{
	case EMHJRotatingDoorState::RDS_Closed:
	case EMHJRotatingDoorState::RDS_Locked:
	case EMHJRotatingDoorState::RDS_Barred:
		DoorTimeline->SetNewTime(0.0f);
		Pivot->SetRelativeRotation(ClosedRotation);
		break;
	case EMHJRotatingDoorState::RDS_Opened:
		DoorTimeline->SetNewTime(1.0f);
		Pivot->SetRelativeRotation(OpenedRotation);
		break;
	}
}

void AMHJRotatingDoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	switch (State)
	{
	case EMHJRotatingDoorState::RDS_Closed:
	case EMHJRotatingDoorState::RDS_Locked:
	case EMHJRotatingDoorState::RDS_Barred:
		Pivot->SetRelativeRotation(ClosedRotation);
		break;
	case EMHJRotatingDoorState::RDS_Opened:
		Pivot->SetRelativeRotation(OpenedRotation);
		break;
	}
	
	FVector Direction = GetActorForwardVector();
	if (bFlipDirection)
	{
		Direction *= -1;
	}
	
#if WITH_EDITORONLY_DATA
	DirectionArrow->SetWorldLocation(GetActorLocation() + Direction * DirectionArrow->ArrowLength * DirectionArrow->ArrowSize);
	DirectionArrow->SetWorldRotation((-Direction).Rotation());
#endif
}

bool AMHJRotatingDoor::CanInteract_Implementation(AActor* Subject) const
{
	return !DoorTimeline->IsPlaying();
}

int32 AMHJRotatingDoor::Interact_Implementation(AActor* Subject)
{
	if (State == EMHJRotatingDoorState::RDS_Locked)
	{
		if (!Key)
		{
			return 1;
		}
		UMHJInventoryComponent* InventoryComponent = Subject->GetComponentByClass<UMHJInventoryComponent>();
		if (!InventoryComponent)
		{
			return 1;
		}
		if (!InventoryComponent->Contains(Key))
		{
			return 1;
		}
		
		if (RemoveKey)
		{
			InventoryComponent->Remove(Key);
		}
		return 2;
	}
	if (State == EMHJRotatingDoorState::RDS_Barred)
	{
		FVector SubjectLocation = Subject->GetActorLocation();
		SubjectLocation.Z = 0.0f;
		const FVector Direction = (GetActorLocation() - SubjectLocation).GetSafeNormal();
		FVector DoorDirection = GetActorForwardVector();
		if (bFlipDirection)
		{
			DoorDirection *= -1.0f;
		}
		const float Dot = FVector::DotProduct(Direction, DoorDirection);
		if (Dot > 0.0f)
		{
			return 1;
		}
		return 2;
	}
	
	return 0;
}

void AMHJRotatingDoor::FinalizeInteraction_Implementation(AActor* Subject, int32 InteractionResult)
{
	switch (InteractionResult)
	{
	case 1:
		if (LockedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), LockedSound, GetActorLocation());
		}
		break;
	case 2:
		Unlock();
		break;
	default:
		if (State == EMHJRotatingDoorState::RDS_Opened)
		{
			Close();
			if (PartnerDoor)
			{
				PartnerDoor->Close();
			}
		}
		else if (State == EMHJRotatingDoorState::RDS_Closed)
		{
			Open();
			if (PartnerDoor && PartnerDoor->CanOpen())
			{
				PartnerDoor->Open();
			}
		}
	}
}

FText AMHJRotatingDoor::GetMessage_Implementation(AActor* Subject, int32 InteractionResult) const
{
	FText Result = FText::GetEmpty();
	switch (InteractionResult)
	{
	case 1:
		if (State == EMHJRotatingDoorState::RDS_Locked)
		{
			Result = Key == nullptr ? LockedMessage : KeyLockedMessage;
		}
		else if (State == EMHJRotatingDoorState::RDS_Barred)
		{
			Result = BarredMessage;
		}
		break;
	case 2:
		Result = UnlockedMessage;
		break;
	default:
		break;
	}
	
	FFormatNamedArguments Arguments;
	Arguments.Add("ItemName", Key ? Key->GetDisplayName() : FText::GetEmpty());
	return FText::Format(Result, Arguments);
}

void AMHJRotatingDoor::MakeOpened()
{
	SmartLink->SetEnabled(true);
	DoorTimeline->Stop();
	DoorTimeline->SetNewTime(1.0f);
	State = EMHJRotatingDoorState::RDS_Opened;
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Key = nullptr;
}

void AMHJRotatingDoor::MakeClosed()
{
	SmartLink->SetEnabled(true);
	DoorTimeline->Stop();
	DoorTimeline->SetNewTime(0.0f);
	State = EMHJRotatingDoorState::RDS_Closed;
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Key = nullptr;
}

void AMHJRotatingDoor::Unlock()
{
	if (CanOpen())
	{
		return;
	}
	
	if (UnlockSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), UnlockSound, GetActorLocation());
	}
	State = EMHJRotatingDoorState::RDS_Closed;
	Key = nullptr;
	
	OnUnlocked.Broadcast();
}

void AMHJRotatingDoor::Lock(UMHJItem* Item)
{
	SmartLink->SetEnabled(true);
	DoorTimeline->Stop();
	DoorTimeline->SetNewTime(0.0f);
	Key = Item;
	
	if (LockedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LockedSound, GetActorLocation());
	}
	State = EMHJRotatingDoorState::RDS_Locked;
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	OnLocked.Broadcast();
}

void AMHJRotatingDoor::Bar()
{
	SmartLink->SetEnabled(true);
	DoorTimeline->Stop();
	DoorTimeline->SetNewTime(0.0f);
	State = EMHJRotatingDoorState::RDS_Barred;
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Key = nullptr;
	
	OnBarred.Broadcast();
}

void AMHJRotatingDoor::Open()
{
	if (State == EMHJRotatingDoorState::RDS_Opened)
	{
		return;
	}
	
	Unlock();
	
	if (OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation());
	}
	State = EMHJRotatingDoorState::RDS_Opened;
	SmartLink->SetEnabled(false);
	DoorTimeline->SetPlayRate(1.0f / OpenDuration);
	DoorTimeline->Play();
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	OnOpened.Broadcast();
}

void AMHJRotatingDoor::Close()
{
	if (State == EMHJRotatingDoorState::RDS_Closed || !CanOpen())
	{
		return;
	}
	
	if (CloseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CloseSound, GetActorLocation());
	}
	State = EMHJRotatingDoorState::RDS_Closed;
	SmartLink->SetEnabled(false);
	DoorTimeline->SetPlayRate(1.0f / CloseDuration);
	DoorTimeline->Reverse();
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	OnClosed.Broadcast();
}

void AMHJRotatingDoor::NotifySmartLinkReached(UNavLinkCustomComponent* Link, UObject* PathingAgent, const FVector& DestPoint)
{
	if (State == EMHJRotatingDoorState::RDS_Opened)
	{
		return;
	}
	
	UPathFollowingComponent* PathComp = Cast<UPathFollowingComponent>(PathingAgent);
	if (!PathComp)
	{
		return;
	}
	
	AActor* PathOwner = PathComp->GetOwner();
	AAIController* ControllerOwner = Cast<AAIController>(PathOwner);
	if (!ControllerOwner)
	{
		return;
	}
	PathOwner = ControllerOwner->GetPawn();
	
	if (!Execute_CanInteract(this, PathOwner))
	{
		PathComp->AbortMove(*this, FPathFollowingResultFlags::MovementStop);
		return;
	}
	
	int32 Result = Execute_Interact(this, PathOwner);
	if (Result != 0)
	{
		PathComp->AbortMove(*this, FPathFollowingResultFlags::Blocked);
		return;
	}
	
	ControllerOwner->SetFocus(this, EAIFocusPriority::Move);
	PathComp->PauseMove();
	
	Execute_FinalizeInteraction(this, PathOwner, Result);
	
	FTimerHandle NavDoorOpenTimerHandle;
	FTimerDelegate NavDoorOpenTimerDelegate;
	NavDoorOpenTimerDelegate.BindUObject(this, &AMHJRotatingDoor::OnNavDoorOpen, TWeakObjectPtr(PathComp));
	GetWorld()->GetTimerManager().SetTimer(
		NavDoorOpenTimerHandle,
		NavDoorOpenTimerDelegate,
		OpenDuration,
		false
		);
}

void AMHJRotatingDoor::OnNavDoorOpen(TWeakObjectPtr<UPathFollowingComponent> PathComp) const
{
	if (!PathComp.IsValid())
	{
		return;
	}
	
	AAIController* ControllerOwner = Cast<AAIController>(PathComp->GetOwner());
	if (ControllerOwner)
	{
		ControllerOwner->ClearFocus(EAIFocusPriority::Move);
	}
	
	PathComp->ResumeMove();
}

void AMHJRotatingDoor::TimelineFloat(float Alpha)
{
	Pivot->SetRelativeRotation(FQuat::Slerp(ClosedRotation.Quaternion(), OpenedRotation.Quaternion(), Alpha));
}

void AMHJRotatingDoor::TimelineFinished()
{
	if (State == EMHJRotatingDoorState::RDS_Opened)
	{
		MakeOpened();
	}
	else if (State == EMHJRotatingDoorState::RDS_Closed)
	{
		MakeClosed();
	}
}
