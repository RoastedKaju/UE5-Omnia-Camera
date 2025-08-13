// Copyright (C) 2025 RoastedKaju. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "OmniaPlayerCameraManager.generated.h"

class FDebugDisplayInfo;
class UCanvas;
class UObject;

#define OMNIA_CAMERA_DEFAULT_FOV		(80.0f)
#define OMNIA_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define OMNIA_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UOmniaUICameraManagerComponent;

/**
 * 
 */
UCLASS(NotPlaceable)
class OMNIACAMERA_API AOmniaPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AOmniaPlayerCameraManager(const FObjectInitializer& ObjectInitializer);
	UOmniaUICameraManagerComponent* GetUICameraComponent() const;

protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

private:
	/** The UI Camera Component, controls the camera when UI is doing something important that gameplay doesn't get priority over. */
	UPROPERTY(Transient)
	TObjectPtr<UOmniaUICameraManagerComponent> UICamera;
	
};
