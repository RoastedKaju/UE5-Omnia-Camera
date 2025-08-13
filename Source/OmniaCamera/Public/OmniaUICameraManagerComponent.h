// Copyright (C) 2025 RoastedKaju. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OmniaUICameraManagerComponent.generated.h"

class AOmniaPlayerCameraManager;

class AActor;
class AHUD;
class APlayerController;
class FDebugDisplayInfo;
class UCanvas;
class UObject;

UCLASS(Transient, Within=OmniaPlayerCameraManager)
class OMNIACAMERA_API UOmniaUICameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	static UOmniaUICameraManagerComponent* GetComponent(APlayerController* PC);
	
	UOmniaUICameraManagerComponent();	
	virtual void InitializeComponent() override;

	bool IsSettingViewTarget() const { return bUpdatingViewTarget; }
	AActor* GetViewTarget() const { return ViewTarget; }
	void SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams());

	bool NeedsToUpdateViewTarget() const;
	void UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime);

	void OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos);

private:
	UPROPERTY(Transient)
	TObjectPtr<AActor> ViewTarget;
	
	UPROPERTY(Transient)
	bool bUpdatingViewTarget;
};
