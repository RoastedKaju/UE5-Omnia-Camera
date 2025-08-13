// Copyright (C) 2025 RoastedKaju. All Rights Reserved.


#include "OmniaPlayerCameraManager.h"

#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "OmniaCameraComponent.h"
#include "OmniaUICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OmniaPlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

AOmniaPlayerCameraManager::AOmniaPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = OMNIA_CAMERA_DEFAULT_FOV;
	ViewPitchMin = OMNIA_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = OMNIA_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UOmniaUICameraManagerComponent>(UICameraComponentName);
}

UOmniaUICameraManagerComponent* AOmniaPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void AOmniaPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AOmniaPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("LyraPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UOmniaCameraComponent* CameraComponent = UOmniaCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}
