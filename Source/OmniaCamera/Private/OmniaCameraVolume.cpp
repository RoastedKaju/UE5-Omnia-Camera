// Copyright (C) 2025 RoastedKaju. All Rights Reserved.


#include "OmniaCameraVolume.h"
#include "OmniaCameraComponent.h"
#include "OmniaCameraMode.h"

AOmniaCameraVolume::AOmniaCameraVolume()
{

}

void AOmniaCameraVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!CameraModeClass)
	{
		return;
	}

	const APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn)
	{
		return;
	}

	if (UOmniaCameraComponent* CameraComponent = UOmniaCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->SetCameraVolume(this);
		CameraComponent->SetVolumeCameraMode(CameraModeClass);
	}
}

void AOmniaCameraVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	const APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn)
	{
		return;
	}

	if (UOmniaCameraComponent* CameraComponent = UOmniaCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->ClearCameraVolume();
		CameraComponent->ClearVolumeCameraMode();
	}
	
	Super::NotifyActorEndOverlap(OtherActor);
}
