// Copyright (c) 2026 Hasmile


#include "Character/MHJPlayerCharacter.h"
#include "Character/MHJCharacterMovementComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AMHJPlayerCharacter::AMHJPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (UMHJCharacterMovementComponent* MovementComponent = GetCharacterMovement<UMHJCharacterMovementComponent>())
	{
		MovementComponent->bCanRun = true;
	}
	
	ViewPitchMin = -89.0f;
	ViewPitchMax = 80.0f;
}

void AMHJPlayerCharacter::EnterCinematic()
{
	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(CinematicInputMappingContext, 0);
	}
}

void AMHJPlayerCharacter::EnterGameplay()
{
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
}

void AMHJPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	EnterGameplay();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMHJPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMHJPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AMHJPlayerCharacter::StartRunning);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMHJPlayerCharacter::StopRunning);
	}
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
