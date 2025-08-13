// Copyright (C) 2025 RoastedKaju. All Rights Reserved.


#include "OmniaUICameraManagerComponent.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "OmniaPlayerCameraManager.h"

class AActor;
class FDebugDisplayInfo;

UOmniaUICameraManagerComponent* UOmniaUICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (AOmniaPlayerCameraManager* PCCamera = Cast<AOmniaPlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

UOmniaUICameraManagerComponent::UOmniaUICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void UOmniaUICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UOmniaUICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<AOmniaPlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UOmniaUICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UOmniaUICameraManagerComponent::UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime)
{
}

void UOmniaUICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
}
