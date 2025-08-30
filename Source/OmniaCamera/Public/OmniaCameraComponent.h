// Copyright (C) 2025 RoastedKaju. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "OmniaCameraComponent.generated.h"

class UCanvas;
class UOmniaCameraMode;
class UOmniaCameraModeStack;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FMinimalViewInfo;
template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UOmniaCameraMode>, FOmniaCameraModeDelegate);

USTRUCT(BlueprintType)
struct FOmniaCameraData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "Omnia|Camera|ThirdPerson")
	float CurrentThirdPersonZoom = 1.0f;
};

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), BlueprintType)
class OMNIACAMERA_API UOmniaCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UOmniaCameraComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Omnia|Camera")
	static UOmniaCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UOmniaCameraComponent>() : nullptr); }

	// Returns the target actor that the camera is looking at.
	virtual AActor* GetTargetActor() const { return GetOwner(); }

	// Delegate used to query for the best camera mode.
	FOmniaCameraModeDelegate DetermineCameraModeDelegate;

	// Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	virtual void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;
	
	UFUNCTION(BlueprintCallable, Category = "Omnia|Camera")
	void SetCameraMode(TSubclassOf<UOmniaCameraMode> CameraModeClass);
	
	UFUNCTION(BlueprintCallable, Category = "Omnia|Camera")
	void ClearCameraMode();

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	FOmniaCameraData CameraData;
	
protected:

	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();

	// Stack used to blend the camera modes.
	UPROPERTY()
	TObjectPtr<UOmniaCameraModeStack> CameraModeStack;
	
	UPROPERTY(EditDefaultsOnly, Category = "Omnia|Camera")
	TSubclassOf<UOmniaCameraMode> DefaultCameraModeClass;
	
	UPROPERTY()
	TSubclassOf<UOmniaCameraMode> ActiveCameraModeClass;
	
	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset;
};