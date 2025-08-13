// Copyright (C) 2025 RoastedKaju. All Rights Reserved.


#include "Modes/OmniaCameraMode_FirstPerson.h"

#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(OmniaCameraMode_FirstPerson)

namespace OmniaCameraMode_FirstPerson_Statics
{
	static const FName NAME_IgnoreCameraCollision = TEXT("IgnoreCameraCollision");
}

void UOmniaCameraMode_FirstPerson::OnActivation_Implementation()
{
	UpdateForTarget(GetWorld()->DeltaTimeSeconds);
	CurrentCrouchOffset = TargetCrouchOffset;
	CrouchOffsetBlendPct = 1.0f;
}

void UOmniaCameraMode_FirstPerson::UpdateView(float DeltaTime)
{
	UpdateForTarget(DeltaTime);
	UpdateCrouchOffset(DeltaTime);

	FVector PivotLocation = GetPivotLocation() + CurrentCrouchOffset;
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
	
	if (bApplyOffset)
	{
		View.Location = PivotLocation + PivotOffset;
	}
}

void UOmniaCameraMode_FirstPerson::UpdateForTarget(float DeltaTime)
{
	if (const ACharacter* TargetCharacter = Cast<ACharacter>(GetTargetActor()))
	{
		if (TargetCharacter->bIsCrouched)
		{
			const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			const float CrouchedHeightAdjustment = TargetCharacterCDO->CrouchedEyeHeight - TargetCharacterCDO->BaseEyeHeight;

			SetTargetCrouchOffset(FVector(0.f, 0.f, CrouchedHeightAdjustment));

			return;
		}
	}

	SetTargetCrouchOffset(FVector::ZeroVector);
}

void UOmniaCameraMode_FirstPerson::DrawDebug(UCanvas* Canvas) const
{
	Super::DrawDebug(Canvas);
}

void UOmniaCameraMode_FirstPerson::SetTargetCrouchOffset(FVector NewTargetOffset)
{
	CrouchOffsetBlendPct = 0.0f;
	InitialCrouchOffset = CurrentCrouchOffset;
	TargetCrouchOffset = NewTargetOffset;
}

void UOmniaCameraMode_FirstPerson::UpdateCrouchOffset(float DeltaTime)
{
	if (CrouchOffsetBlendPct < 1.0f)
	{
		CrouchOffsetBlendPct = FMath::Min(CrouchOffsetBlendPct + DeltaTime * CrouchOffsetBlendMultiplier, 1.0f);
		CurrentCrouchOffset = FMath::InterpEaseInOut(InitialCrouchOffset, TargetCrouchOffset, CrouchOffsetBlendPct, 1.0f);
	}
	else
	{
		CurrentCrouchOffset = TargetCrouchOffset;
		CrouchOffsetBlendPct = 1.0f;
	}
}
