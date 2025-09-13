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
template <class TClass>
class TSubclassOf;
class AOmniaCameraVolume;

DECLARE_LOG_CATEGORY_EXTERN(OmniaCameraComponentLog, Log, All);

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

	// Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	virtual void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

	UFUNCTION(BlueprintCallable, Category = "Omnia|Camera")
	void SetCameraMode(TSubclassOf<UOmniaCameraMode> CameraModeClass);

	UFUNCTION(BlueprintCallable, Category = "Omnia|Camera")
	void ClearCameraMode();

	UFUNCTION(BlueprintCallable, Category = "Omnia|Camera")
	void SetVolumeCameraMode(TSubclassOf<UOmniaCameraMode> CameraModeClass);

	UFUNCTION(BlueprintCallable, Category = "Omnia|Camera")
	void ClearVolumeCameraMode();

	UFUNCTION(BlueprintCallable, Category = "Omnia|Camera")
	void SetCameraVolume(AOmniaCameraVolume* CameraVolume);

	UFUNCTION(BlueprintCallable, Category = "Omnia|Camera")
	void ClearCameraVolume();

	UFUNCTION(BlueprintPure, Category = "Omnia|Camera")
	AOmniaCameraVolume* GetCameraVolume() const;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	FOmniaCameraData CameraData;

protected:
	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();

	// Stack used to blend the camera modes.
	UPROPERTY(BlueprintReadOnly, Category = "Omnia|Camera")
	TObjectPtr<UOmniaCameraModeStack> CameraModeStack;

	UPROPERTY(EditDefaultsOnly, Category = "Omnia|Camera")
	TSubclassOf<UOmniaCameraMode> DefaultCameraModeClass;

	UPROPERTY()
	TSubclassOf<UOmniaCameraMode> ActiveCameraModeClass;

	UPROPERTY()
	TSubclassOf<UOmniaCameraMode> VolumeCameraModeClass;

	// Runtime current camera volume
	UPROPERTY(Transient)
	TWeakObjectPtr<AOmniaCameraVolume> CurrentCameraVolume;

	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset;
};
