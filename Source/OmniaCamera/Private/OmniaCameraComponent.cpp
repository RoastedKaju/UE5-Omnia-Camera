// Copyright (C) 2025 RoastedKaju. All Rights Reserved.


#include "OmniaCameraComponent.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "OmniaCameraMode.h"
#include "OmniaCameraVolume.h"
#include "Modes/OmniaCameraMode_Static.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OmniaCameraComponent)

DEFINE_LOG_CATEGORY(OmniaCameraComponentLog);

UOmniaCameraComponent::UOmniaCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CameraModeStack = nullptr;
	FieldOfViewOffset = 0.0f;
}

void UOmniaCameraComponent::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("LyraCameraComponent: %s"), *GetNameSafe(GetTargetActor())));

	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   Location: %s"), *GetComponentLocation().ToCompactString()));
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   Rotation: %s"), *GetComponentRotation().ToCompactString()));
	DisplayDebugManager.DrawString(FString::Printf(TEXT("   FOV: %f"), FieldOfView));

	check(CameraModeStack);
	CameraModeStack->DrawDebug(Canvas);
}

void UOmniaCameraComponent::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
	check(CameraModeStack);
	CameraModeStack->GetBlendInfo(/*out*/ OutWeightOfTopLayer, /*out*/ OutTagOfTopLayer);
}

void UOmniaCameraComponent::SetCameraMode(TSubclassOf<UOmniaCameraMode> CameraModeClass)
{
	if (CameraModeClass)
	{
		DefaultCameraModeClass = CameraModeClass;
	}
}

void UOmniaCameraComponent::ClearCameraMode()
{
	DefaultCameraModeClass = nullptr;
}

void UOmniaCameraComponent::SetVolumeCameraMode(TSubclassOf<UOmniaCameraMode> CameraModeClass)
{
	if (CameraModeClass)
	{
		VolumeCameraModeClass = CameraModeClass;
	}
}

void UOmniaCameraComponent::ClearVolumeCameraMode()
{
	VolumeCameraModeClass = nullptr;
}

void UOmniaCameraComponent::SetCameraVolume(AOmniaCameraVolume* CameraVolume)
{
	CurrentCameraVolume = CameraVolume;
}

void UOmniaCameraComponent::ClearCameraVolume()
{
	CurrentCameraVolume = nullptr;
}

AOmniaCameraVolume* UOmniaCameraComponent::GetCameraVolume() const
{
	return CurrentCameraVolume.Get();
}

void UOmniaCameraComponent::OnRegister()
{
	Super::OnRegister();

	if (!CameraModeStack)
	{
		CameraModeStack = NewObject<UOmniaCameraModeStack>(this);
		check(CameraModeStack);
	}
}

void UOmniaCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();

	FOmniaCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	// Keep player controller in sync with the latest view.
	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Apply any offset that was added to the field of view.
	CameraModeView.FieldOfView += FieldOfViewOffset;
	FieldOfViewOffset = 0.0f;

	// Keep camera component in sync with the latest view.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);
	FieldOfView = CameraModeView.FieldOfView;

	// Fill in desired view.
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;

	// See if the CameraActor wants to override the PostProcess settings used.
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}

	if (IsXRHeadTrackedCamera())
	{
		// In XR much of the camera behavior above is irrelevant, but the post process settings are not.
		Super::GetCameraView(DeltaTime, DesiredView);
	}
}

void UOmniaCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (CameraModeStack->IsStackActivate())
	{
		// Determine the camera mode
		if (IsValid(VolumeCameraModeClass))
		{
			ActiveCameraModeClass = VolumeCameraModeClass;
			CameraModeStack->PushCameraMode(ActiveCameraModeClass);
			return;
		}
		if (IsValid(DefaultCameraModeClass))
		{
			ActiveCameraModeClass = DefaultCameraModeClass;
			CameraModeStack->PushCameraMode(ActiveCameraModeClass);
		}
	}
}
