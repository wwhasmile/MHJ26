// Copyright (c) 2026 Hasmile


#include "Player/MHJPlayerCharacter.h"
#include "Character/MHJCharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Interaction/MHJInteractionComponent.h"
#include "Inventory/MHJInventoryComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

const FString AMHJPlayerCharacter::PlayerCharacterName("PlayerCharacter");

const FName AMHJPlayerCharacter::FirstPersonCameraComponentName("FPCameraComp");
const FName AMHJPlayerCharacter::FirstPersonInteractionComponentName("FPInteractionComp");
const FName AMHJPlayerCharacter::InventoryComponentName("InventoryComp");

const FName AMHJPlayerCharacter::FirstPersonCameraSocketName("CameraSocket");

AMHJPlayerCharacter::AMHJPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PreDecayDelay = 8.0f;
	
	BaseFieldOfView = 80.0f;
	RunningFieldOfViewModifier = 100.0f / 80.0f;
	RunningFieldOfViewInterpolationSpeed = 4.0f;
	WalkingFieldOfViewInterpolationSpeed = 9.0f;
	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FirstPersonCameraComponentName);
	FirstPersonCamera->FieldOfView = BaseFieldOfView;
	FirstPersonCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FirstPersonCameraSocketName);
	FirstPersonInteraction = CreateDefaultSubobject<UMHJInteractionComponent>(FirstPersonInteractionComponentName);
	FirstPersonInteraction->ArmLength = 120.0f;
	FirstPersonInteraction->ArmRadius = 2.0f;
	FirstPersonInteraction->AttachToComponent(FirstPersonCamera, FAttachmentTransformRules::KeepRelativeTransform);
	
	Inventory = CreateDefaultSubobject<UMHJInventoryComponent>(InventoryComponentName);
	
	if (UMHJCharacterMovementComponent* MovementComponent = GetCharacterMovement<UMHJCharacterMovementComponent>())
	{
		MovementComponent->bCanRun = true;
		MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
	ViewPitchMin = -89.0f;
	ViewPitchMax = 80.0f;
}

void AMHJPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (FirstPersonCamera)
	{
		bool bIsRunning = false;
		if (UMHJCharacterMovementComponent* MovementComponent = GetCharacterMovement<UMHJCharacterMovementComponent>())
		{
			float Speed = FVector2D(GetVelocity().X, GetVelocity().Y).SquaredLength();
			bIsRunning = !bDead && MovementComponent->IsRunning() && Speed > MovementComponent->MaxWalkSpeed * MovementComponent->MaxWalkSpeed;
		}
		float DesiredFieldOfView = bIsRunning ? BaseFieldOfView * RunningFieldOfViewModifier : BaseFieldOfView;
		float DesiredInterpolationSpeed = bIsRunning ? RunningFieldOfViewInterpolationSpeed : WalkingFieldOfViewInterpolationSpeed;
		FirstPersonCamera->FieldOfView = FMath::FInterpTo(FirstPersonCamera->FieldOfView, DesiredFieldOfView, DeltaTime, DesiredInterpolationSpeed);
	}
}

void AMHJPlayerCharacter::EnterCinematic()
{
	if (bDead)
	{
		return;
	}
	
	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(CinematicInputMappingContext, 0);
		
		if (UMHJCharacterMovementComponent* MovementComponent = GetCharacterMovement<UMHJCharacterMovementComponent>())
		{
			MovementComponent->StopRunning();
		}
		UnCrouch();
	}
	
	if (IsValid(FirstPersonInteraction))
	{
		FirstPersonInteraction->Deactivate();
	}
}

void AMHJPlayerCharacter::EnterGameplay()
{
	if (bDead)
	{
		return;
	}
	
	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		
		APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
		const APlayerCameraManager* DefaultPlayerCameraManager = PlayerCameraManager->GetClass()->GetDefaultObject<APlayerCameraManager>();
		PlayerCameraManager->ViewYawMin = DefaultPlayerCameraManager->ViewYawMin;
		PlayerCameraManager->ViewYawMax = DefaultPlayerCameraManager->ViewYawMax;
		PlayerCameraManager->ViewPitchMin = ViewPitchMin;
		PlayerCameraManager->ViewPitchMax = ViewPitchMax;
		
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(GeneralInputMappingContext, 0);
	}
	
	if (IsValid(FirstPersonInteraction))
	{
		FirstPersonInteraction->Activate();
	}
}

FString AMHJPlayerCharacter::OverrideName_Implementation() const
{
	return PlayerCharacterName;
}

void AMHJPlayerCharacter::SpudStoreCustomData_Implementation(const USpudState* State, USpudStateCustomData* CustomData)
{
	TArray<FPrimaryAssetId> Slots = Inventory->GetSlotsSerialized();
	CustomData->WriteInt(Slots.Num());
	for (const FPrimaryAssetId& SlotId : Slots)
	{
		CustomData->WriteString(SlotId.PrimaryAssetName.ToString());
	}
}

void AMHJPlayerCharacter::SpudRestoreCustomData_Implementation(USpudState* State, USpudStateCustomData* CustomData)
{
	TArray<FPrimaryAssetId> Slots;
	int32 Num;
	CustomData->ReadInt(Num);
	Slots.Reserve(Num);
	for (int32 i = 0; i < Num; ++i)
	{
		FString NameString;
		CustomData->ReadString(NameString);
		Slots.Add(FPrimaryAssetId("Item", FName(NameString)));
	}
	Inventory->LoadSlotsSerialized(Slots);
}

void AMHJPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	EnterGameplay();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMHJPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMHJPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMHJPlayerCharacter::StartCrouching);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMHJPlayerCharacter::StopCrouching);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AMHJPlayerCharacter::StartRunning);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMHJPlayerCharacter::StopRunning);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMHJPlayerCharacter::Interact);
	}
}

float AMHJPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (!bDead)
	{
		if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			Subsystem->ClearAllMappings();
		}
		
		bDead = true;
		if (IsValid(FirstPersonInteraction))
		{
			FirstPersonInteraction->Deactivate();
		}
		
		Death(DamageEvent.DamageTypeClass);
		OnDeath.Broadcast(DamageEvent.DamageTypeClass);
		GetWorldTimerManager().SetTimer(DecayTimerHandle, this, &AMHJPlayerCharacter::Decay, PreDecayDelay, false);
	}
	return Damage;
}

void AMHJPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerPitchInput(Axis.Y);
	AddControllerYawInput(Axis.X);
}

void AMHJPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	
	if (const AController* CharacterController = GetController())
	{
		const FRotator ControlRotation = CharacterController->GetControlRotation();
		const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
	
		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
		AddMovementInput(ForwardVector, Axis.Y);
		AddMovementInput(RightVector, Axis.X);
	}
}

void AMHJPlayerCharacter::StartCrouching(const FInputActionValue& Value)
{
	Crouch();
}

void AMHJPlayerCharacter::StopCrouching(const FInputActionValue& Value)
{
	UnCrouch();
}

void AMHJPlayerCharacter::StartRunning(const FInputActionValue& Value)
{
	if (UMHJCharacterMovementComponent* MovementComponent = GetCharacterMovement<UMHJCharacterMovementComponent>())
	{
		MovementComponent->StartRunning();
	}
}

void AMHJPlayerCharacter::StopRunning(const FInputActionValue& Value)
{
	if (UMHJCharacterMovementComponent* MovementComponent = GetCharacterMovement<UMHJCharacterMovementComponent>())
	{
		MovementComponent->StopRunning();
	}
}

void AMHJPlayerCharacter::Interact(const FInputActionValue& Value)
{
	if (FirstPersonInteraction)
	{
		FirstPersonInteraction->Interact();
	}
}

void AMHJPlayerCharacter::Decay()
{
	Destroy();
}

void AMHJPlayerCharacter::Suicide()
{
	TakeDamage(1000.0f, FDamageEvent(), GetController(), this);
}


