// Copyright (C) 2025 RoastedKaju. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OmniaCameraMode.h"
#include "OmniaCameraMode_Static.generated.h"

class AOmniaCameraVolume;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class OMNIACAMERA_API UOmniaCameraMode_Static : public UOmniaCameraMode
{
	GENERATED_BODY()

public:
	void SetCameraVolume(AOmniaCameraVolume* Volume);

protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<AOmniaCameraVolume> CameraVolume;
};
