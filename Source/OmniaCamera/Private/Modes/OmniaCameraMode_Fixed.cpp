// Copyright (C) 2025 RoastedKaju. All Rights Reserved.


#include "Modes/OmniaCameraMode_Fixed.h"
#include "OmniaCameraComponent.h"
#include "OmniaCameraVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RotationMatrix.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(OmniaCameraMode_Fixed)

UOmniaCameraMode_Fixed::UOmniaCameraMode_Fixed()
{
}

void UOmniaCameraMode_Fixed::OnActivation_Implementation()
{
	CameraOwner = GetOmniaCameraComponent()->GetOwner();

	// Starting previous rotation
	if (CameraVolume.Get() && CameraVolume->FixedCameraPoint)
	{
		PreviousDesiredRot = CameraVolume->FixedCameraPoint->GetActorRotation();
	}
}

void UOmniaCameraMode_Fixed::UpdateView(float DeltaTime)
{
	if (!CameraVolume.Get() || !CameraVolume->FixedCameraPoint)
	{
		return;
	}
	const AActor* ViewPoint = CameraVolume.Get()->FixedCameraPoint;

	FVector DesiredLocation = ViewPoint->GetActorLocation();
	FRotator DesiredRotation = ViewPoint->GetActorRotation();

	// Camera Focus Lag
	if (bFocusOnPlayer)
	{
		const FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(DesiredLocation, CameraOwner->GetActorLocation());
		if (CameraRotationLagSpeed > 0.f)
		{
			DesiredRotation = FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(TargetRot), DeltaTime, CameraRotationLagSpeed).Rotator();
		}
		else
		{
			DesiredRotation = TargetRot;
		}
	}

	View.Location = DesiredLocation;
	View.Rotation = DesiredRotation;
	View.ControlRotation = ViewPoint->GetActorRotation();
	View.FieldOfView = FieldOfView;

	// Previous rotation
	PreviousDesiredRot = DesiredRotation;
}
