// Copyright (c) 2026 Hasmile

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "MHJAnimNotify_Footstep.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class MHJ26_API UMHJAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	static const FName SoundSurfaceTypeParameterName;
	static const FName SoundIsRunningParameterName;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Detection", meta=(AllowPrivateAccess=true))
	FName SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Detection", meta=(AllowPrivateAccess=true))
	FVector Start;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Detection", meta=(AllowPrivateAccess=true))
	float Distance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI", meta=(UIMin=0, ClampMin=0, UIMax=1, ClampMax=1, AllowPrivateAccess=true))
	float NoiseLoudness;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI", meta=(UIMin=0, ClampMin=0, EditCondition="NoiseLoudness>0", AllowPrivateAccess=true))
	float NoiseMaxRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI", meta=(AllowPrivateAccess=true))
	bool bIsRunning;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound", meta=(AllowPrivateAccess=true))
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USoundBase>> SoundMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sound", meta=(AllowPrivateAccess=true))
	float Volume;
	
public:
	UMHJAnimNotify_Footstep();
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
