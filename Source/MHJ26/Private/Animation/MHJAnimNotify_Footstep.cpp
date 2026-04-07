// Copyright (c) 2026 Hasmile


#include "Animation/MHJAnimNotify_Footstep.h"

#include "KismetTraceUtils.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

const FName UMHJAnimNotify_Footstep::SoundSurfaceTypeParameterName("SurfaceType");
const FName UMHJAnimNotify_Footstep::SoundIsRunningParameterName("bIsRunning");

UMHJAnimNotify_Footstep::UMHJAnimNotify_Footstep()
{
	Volume = 1.0f;
	Distance = 88.0f;
}

void UMHJAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}
	UWorld* World = MeshComp->GetWorld();
	if (!World)
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	
	Super::Notify(MeshComp, Animation, EventReference);
	
	FVector StartLocation = Start;
	const USkeletalMeshSocket* Socket;
	if (SocketName.IsValid())
	{
		Socket = MeshComp->GetSocketByName(SocketName);
	}
	else
	{
		Socket = nullptr;
	}
	
	if (Socket)
	{
		StartLocation += Socket->GetSocketLocation(MeshComp);
	}
	else
	{
		StartLocation += MeshComp->GetComponentLocation();
	}
	FVector EndLocation = StartLocation + FVector::DownVector * Distance;
	
	FHitResult Hit;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Owner);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	bool bHit = World->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);
#if !UE_BUILD_SHIPPING
	DrawDebugLineTraceSingle(World, StartLocation, EndLocation, EDrawDebugTrace::ForDuration, bHit, Hit, FLinearColor::Red, FLinearColor::Green, 2.0f);
#endif
	
	if (bHit)
	{
		if (SoundMap.Num() > 0)
		{
			EPhysicalSurface SurfaceType = EPhysicalSurface::SurfaceType_Default;
			if (Hit.PhysMaterial.IsValid())
			{
				SurfaceType = Hit.PhysMaterial->SurfaceType;
			}
			
			TObjectPtr<USoundBase>* SoundPtr = SoundMap.Find(SurfaceType);
			if (!SoundPtr && SurfaceType != EPhysicalSurface::SurfaceType_Default)
			{
				SoundPtr = SoundMap.Find(EPhysicalSurface::SurfaceType_Default);
			}
			if (SoundPtr)
			{
				UGameplayStatics::PlaySoundAtLocation(World, *SoundPtr, Hit.ImpactPoint, Volume);
			}
		}
		
		if (APawn* Pawn = Cast<APawn>(Owner))
		{
			Pawn->MakeNoise(NoiseLoudness, Pawn, Hit.ImpactPoint, NoiseMaxRange);
		}
	}
}
