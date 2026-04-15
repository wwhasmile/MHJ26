// Copyright (c) 2026 Hasmile

#pragma once

#include "ISpudObject.h"
#include "GameFramework/Actor.h"
#include "Interaction/MHJInteractable.h"
#include "MHJRotatingDoor.generated.h"

class UMHJItem;
class USceneComponent;
class UStaticMeshComponent;
class UTimelineComponent;

#if WITH_EDITORONLY_DATA
class UArrowComponent;
#endif

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMHJRotatingDoorUnlockedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMHJRotatingDoorLockedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMHJRotatingDoorBarredSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMHJRotatingDoorOpenedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMHJRotatingDoorClosedSignature);

UENUM(BlueprintType)
enum class EMHJRotatingDoorState : uint8
{
	RDS_Closed UMETA(DisplayName = "Closed"),
	RDS_Opened UMETA(DisplayName = "Opened"),
	RDS_Locked UMETA(DisplayName = "Locked"),
	RDS_Barred UMETA(DisplayName = "Barred"),
};

UCLASS(Abstract)
class MHJ26_API AMHJRotatingDoor : public AActor, public IMHJInteractable, public ISpudObject
{
	GENERATED_BODY()
	
public:
	static FName RootPivotComponentName;
	static FName PivotComponentName;
	static FName DoorMeshComponentName;
	static FName DoorTimelineComponentName;
	
private:
	static FName DoorTimelineAlphaTrackName;
	
#if WITH_EDITORONLY_DATA
	static FName DirectionArrowComponentName;
#endif
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door", SaveGame, meta=(AllowPrivateAccess=true))
	EMHJRotatingDoorState State;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", SaveGame)
	TObjectPtr<AMHJRotatingDoor> PartnerDoor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door", SaveGame, meta=(EditCondition="State == EMHJRotatingDoorState::RDS_Locked"))
	TObjectPtr<UMHJItem> Key;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door", meta=(EditCondition="State == EMHJRotatingDoorState::RDS_Locked"))
	bool RemoveKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	bool bFlipDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText LockedMessage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText KeyLockedMessage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText BarredMessage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText UnlockedMessage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	FRotator OpenedRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	FRotator ClosedRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	FVector OutsideVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (ClampMin=0.0f, UIMin=0.0f, ForceUnits=Second))
	float OpenDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (ClampMin=0.0f, UIMin=0.0f, ForceUnits=Second))
	float CloseDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> OpenSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> CloseSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> LockedSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> UnlockSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UCurveFloat> Curve;
	
	UPROPERTY(BlueprintAssignable, Category = "Door")
	FMHJRotatingDoorUnlockedSignature OnUnlocked;
	UPROPERTY(BlueprintAssignable, Category = "Door")
	FMHJRotatingDoorLockedSignature OnLocked;
	UPROPERTY(BlueprintAssignable, Category = "Door")
	FMHJRotatingDoorLockedSignature OnBarred;
	UPROPERTY(BlueprintAssignable, Category = "Door")
	FMHJRotatingDoorOpenedSignature OnOpened;
	UPROPERTY(BlueprintAssignable, Category = "Door")
	FMHJRotatingDoorClosedSignature OnClosed;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> RootPivot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> Pivot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<UStaticMeshComponent> DoorMesh;
	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> DoorTimeline;
	
#if WITH_EDITORONLY_DATA
	TObjectPtr<UArrowComponent> DirectionArrow;
#endif

public:
	AMHJRotatingDoor();
	
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual int32 Interact_Implementation(AActor* Subject) override;
	virtual void FinalizeInteraction_Implementation(AActor* Subject, int32 InteractionResult) override;
	virtual FText GetMessage_Implementation(AActor* Subject, int32 InteractionResult) const override;
	
	UFUNCTION(BlueprintCallable, Category="Door")
	void MakeOpened();
	UFUNCTION(BlueprintCallable, Category="Door")
	void MakeClosed();
	
	UFUNCTION(BlueprintCallable, Category="Door")
	void Unlock();
	UFUNCTION(BlueprintCallable, Category="Door")
	void Lock(UMHJItem* Item = nullptr);
	UFUNCTION(BlueprintCallable, Category="Door")
	void Bar();
	
	UFUNCTION(BlueprintCallable, Category="Door")
	void Open();
	UFUNCTION(BlueprintCallable, Category="Door")
	void Close();
	
	UFUNCTION(BlueprintPure, Category="Door")
	FORCEINLINE bool CanOpen() const { return State != EMHJRotatingDoorState::RDS_Locked && State != EMHJRotatingDoorState::RDS_Barred; }
	
	FORCEINLINE EMHJRotatingDoorState GetState() const { return State; }
	
private:
	UFUNCTION()
	void TimelineFloat(float Alpha);

	UFUNCTION()
	void TimelineFinished();
	
};
