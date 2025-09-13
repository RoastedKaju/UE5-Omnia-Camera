// Copyright (C) 2025 RoastedKaju. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/World.h"
#include "GameplayTagContainer.h"
#include "OmniaCameraMode.generated.h"

class AActor;
class UCanvas;
class UOmniaCameraComponent;

/**
 * EOmniaCameraModeBlendFunction
 *
 *	Blend function used for transitioning between camera modes.
 */
UENUM(BlueprintType)
enum class EOmniaCameraModeBlendFunction : uint8
{
	// Does a simple linear interpolation.
	Linear,

	// Immediately accelerates, but smoothly decelerates into the target.  Ease amount controlled by the exponent.
	EaseIn,

	// Smoothly accelerates, but does not decelerate into the target.  Ease amount controlled by the exponent.
	EaseOut,

	// Smoothly accelerates and decelerates.  Ease amount controlled by the exponent.
	EaseInOut,

	COUNT UMETA(Hidden)
};

/**
 * FLyraCameraModeView
 *
 *	View data produced by the camera mode that is used to blend camera modes.
 */
struct FOmniaCameraModeView
{
	FOmniaCameraModeView();

	void Blend(const FOmniaCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/**
 * UOmniaCameraMode
 *
 *	Base class for all camera modes.
 */
UCLASS(Abstract, NotBlueprintable)
class OMNIACAMERA_API UOmniaCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UOmniaCameraMode();

	UOmniaCameraComponent* GetOmniaCameraComponent() const;

	virtual UWorld* GetWorld() const override;

	AActor* GetTargetActor() const;

	const FOmniaCameraModeView& GetCameraModeView() const { return View; }

	// Called when this camera mode is activated on the camera mode stack.
	UFUNCTION(BlueprintNativeEvent, Category = "Omnia|Camera", meta=(DisplayName="On Activation"))
	void OnActivation();

	virtual void OnActivation_Implementation()
	{
	};

	// Called when this camera mode is deactivated on the camera mode stack.
	UFUNCTION(BlueprintNativeEvent, Category = "Omnia|Camera", meta=(DisplayName="On Deactivation"))
	void OnDeactivation();

	virtual void OnDeactivation_Implementation()
	{
	};

	void UpdateCameraMode(float DeltaTime);

	/** 
	 * Blueprint hook to allow blueprints to override existing camera behavior or implement custom cameras.
	 * If this function returns true, we will use the given returned values and skip further calculations to determine
	 * final camera POV. 
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category="Omnia|Camera", meta=(DisplayName="Blueprint Update Camera"))
	bool BlueprintUpdateCamera(FVector& OutLocation, FRotator& OutRotation, FRotator& OutControlRotation, float& OutFieldOfView);

	float GetBlendTime() const { return BlendTime; }
	float GetBlendWeight() const { return BlendWeight; }
	void SetBlendWeight(float Weight);

	FGameplayTag GetCameraTypeTag() const
	{
		return CameraTypeTag;
	}

	virtual void DrawDebug(UCanvas* Canvas) const;

protected:
	UFUNCTION(BlueprintPure, Category="Omnia|Camera")
	virtual FVector GetPivotLocation() const;
	UFUNCTION(BlueprintPure, Category="Omnia|Camera")
	virtual FRotator GetPivotRotation() const;

	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);

	// A tag that can be queried by gameplay code that cares when a kind of camera mode is active
	// without having to ask about a specific mode (e.g., when aiming downsights to get more accuracy)
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	FGameplayTag CameraTypeTag;

	// View output produced by the camera mode.
	FOmniaCameraModeView View;

	// The horizontal field of view (in degrees).
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	// Minimum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	// Maximum view pitch (in degrees).
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	// How long it takes to blend in this mode.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// Function used for blending.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	EOmniaCameraModeBlendFunction BlendFunction;

	// Exponent used by blend functions to control the shape of the curve.
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	// Linear blend alpha used to determine the blend weight.
	float BlendAlpha;

	// Blend weight calculated using the blend alpha and function.
	float BlendWeight;

	/** If true, skips all interpolation and puts camera in ideal location.  Automatically set to false next frame. */
	UPROPERTY(transient)
	uint32 bResetInterpolation : 1;
};

/**
 * UOmniaCameraModeStack
 *
 *	Stack used for blending camera modes.
 */
UCLASS()
class UOmniaCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	UOmniaCameraModeStack();

	void ActivateStack();
	void DeactivateStack();

	UFUNCTION(BlueprintPure, Category="Omnia|Camera")
	bool IsStackActivate() const { return bIsActive; }

	void PushCameraMode(TSubclassOf<UOmniaCameraMode> CameraModeClass);

	bool EvaluateStack(float DeltaTime, FOmniaCameraModeView& OutCameraModeView);

	void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:
	UFUNCTION(BlueprintCallable, Category="Omnia|Camera")
	UOmniaCameraMode* GetCameraModeInstance(TSubclassOf<UOmniaCameraMode> CameraModeClass);

	void UpdateStack(float DeltaTime);
	void BlendStack(FOmniaCameraModeView& OutCameraModeView) const;

	bool bIsActive;

	UPROPERTY()
	TArray<TObjectPtr<UOmniaCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UOmniaCameraMode>> CameraModeStack;
};
