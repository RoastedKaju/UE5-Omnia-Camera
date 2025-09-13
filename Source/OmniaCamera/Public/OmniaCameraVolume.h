// Copyright (C) 2025 RoastedKaju. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OmniaCameraVolume.generated.h"

class UOmniaCameraMode;

UCLASS()
class OMNIACAMERA_API AOmniaCameraVolume : public AVolume
{
	GENERATED_BODY()

public:
	AOmniaCameraVolume();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Omnia|Camera")
	TObjectPtr<AActor> FixedCameraPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Omnia|Camera")
	TSubclassOf<UOmniaCameraMode> CameraModeClass;
};
