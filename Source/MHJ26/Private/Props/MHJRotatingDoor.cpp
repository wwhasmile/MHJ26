// Copyright (c) 2026 Hasmile


#include "Props/MHJRotatingDoor.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Inventory/MHJInventoryComponent.h"
#include "Kismet/GameplayStatics.h"


FName AMHJRotatingDoor::ColliderComponentName("ColliderComp");
FName AMHJRotatingDoor::PivotComponentName("PivotComp");
FName AMHJRotatingDoor::DoorMeshComponentName("DoorMeshComp");
FName AMHJRotatingDoor::DoorTimelineComponentName("DoorTimelineComp");

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
	
	Collider = CreateDefaultSubobject<UBoxComponent>(ColliderComponentName);
	SetRootComponent(Collider);
	
	Pivot = CreateDefaultSubobject<USceneComponent>(PivotComponentName);
	Pivot->SetupAttachment(Collider);
	
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(DoorMeshComponentName);
	DoorMesh->SetupAttachment(Pivot);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(DoorTimelineComponentName);
	
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
		Pivot->SetRelativeRotation(ClosedRotation);
		break;
	case EMHJRotatingDoorState::RDS_Opened:
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
		}
		else if (State == EMHJRotatingDoorState::RDS_Closed)
		{
			Open();
		}
	}
}

FText AMHJRotatingDoor::GetMessage_Implementation(AActor* Subject, int32 InteractionResult) const
{
	switch (InteractionResult)
	{
	case 1:
		if (State == EMHJRotatingDoorState::RDS_Locked)
		{
			return Key == nullptr ? LockedMessage : KeyLockedMessage;
		}
		if (State == EMHJRotatingDoorState::RDS_Barred)
		{
			return BarredMessage;
		}
	case 2:
		return UnlockedMessage;
	default:
		return FText::GetEmpty();
	}
}

void AMHJRotatingDoor::MakeOpened()
{
	DoorTimeline->Stop();
	DoorTimeline->SetNewTime(0.0f);
	State = EMHJRotatingDoorState::RDS_Opened;
	Pivot->SetRelativeRotation(OpenedRotation);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMHJRotatingDoor::MakeClosed()
{
	DoorTimeline->Stop();
	DoorTimeline->SetNewTime(0.0f);
	State = EMHJRotatingDoorState::RDS_Closed;
	Pivot->SetRelativeRotation(ClosedRotation);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
}

void AMHJRotatingDoor::Lock(UMHJItem* Item)
{
	DoorTimeline->Stop();
	DoorTimeline->SetNewTime(0.0f);
	Key = Item;
	State = EMHJRotatingDoorState::RDS_Locked;
	Pivot->SetRelativeRotation(ClosedRotation);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMHJRotatingDoor::Bar()
{
	DoorTimeline->Stop();
	DoorTimeline->SetNewTime(0.0f);
	State = EMHJRotatingDoorState::RDS_Barred;
	Pivot->SetRelativeRotation(ClosedRotation);
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
	DoorTimeline->SetPlayRate(1.0f / OpenDuration);
	DoorTimeline->Play();
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	DoorTimeline->SetPlayRate(1.0f / CloseDuration);
	DoorTimeline->Reverse();
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMHJRotatingDoor::TimelineFloat(float Alpha)
{
	Pivot->SetRelativeRotation(FQuat::Slerp(ClosedRotation.Quaternion(), OpenedRotation.Quaternion(), Alpha));
}

void AMHJRotatingDoor::TimelineFinished()
{
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
