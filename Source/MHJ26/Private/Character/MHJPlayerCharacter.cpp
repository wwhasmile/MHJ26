// Copyright (c) 2026 Hasmile


#include "Character/MHJPlayerCharacter.h"
#include "Character/MHJCharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Interaction/MHJInteractionComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

FName AMHJPlayerCharacter::FirstPersonCameraComponentName = FName("FPCameraComp");
FName AMHJPlayerCharacter::FirstPersonInteractionComponentName = FName("FPInteractionComp");

AMHJPlayerCharacter::AMHJPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BaseFieldOfView = 80.0f;
	RunningFieldOfViewModifier = 100.0f / 80.0f;
	RunningFieldOfViewInterpolationSpeed = 4.0f;
	WalkingFieldOfViewInterpolationSpeed = 9.0f;
	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FirstPersonCameraComponentName);
	FirstPersonCamera->FieldOfView = BaseFieldOfView;
	FirstPersonCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	FirstPersonInteraction = CreateDefaultSubobject<UMHJInteractionComponent>(FirstPersonInteractionComponentName);
	FirstPersonInteraction->AttachToComponent(FirstPersonCamera, FAttachmentTransformRules::KeepRelativeTransform);
	
	if (UMHJCharacterMovementComponent* MovementComponent = GetCharacterMovement<UMHJCharacterMovementComponent>())
	{
		MovementComponent->bCanRun = true;
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
			bIsRunning = MovementComponent->IsRunning() && Speed > MovementComponent->MaxWalkSpeed * MovementComponent->MaxWalkSpeed;
		}
		float DesiredFieldOfView = bIsRunning ? BaseFieldOfView * RunningFieldOfViewModifier : BaseFieldOfView;
		float DesiredInterpolationSpeed = bIsRunning ? RunningFieldOfViewInterpolationSpeed : WalkingFieldOfViewInterpolationSpeed;
		FirstPersonCamera->FieldOfView = FMath::FInterpTo(FirstPersonCamera->FieldOfView, DesiredFieldOfView, DeltaTime, DesiredInterpolationSpeed);
	}
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
