// Copyright (C) 2025 RoastedKaju. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OmniaCameraMode.h"
#include "OmniaCameraMode_FirstPerson.generated.h"

class UCurveVector;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class OMNIACAMERA_API UOmniaCameraMode_FirstPerson : public UOmniaCameraMode
{
	GENERATED_BODY()

protected:
	virtual void OnActivation_Implementation() override;
	
	virtual void UpdateView(float DeltaTime) override;

	void UpdateForTarget(float DeltaTime);

	virtual void DrawDebug(UCanvas* Canvas) const override;

	// Alters the speed that a crouch offset is blended in or out
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "First Person")
	float CrouchOffsetBlendMultiplier = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "First Person")
	bool bApplyOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "First Person", meta= (EditCondition = "bApplyOffset"))
	FVector PivotOffset;

#if ENABLE_DRAW_DEBUG
	mutable float LastDrawDebugTime = -MAX_FLT;
#endif
	
	void SetTargetCrouchOffset(FVector NewTargetOffset);
	void UpdateCrouchOffset(float DeltaTime);

	FVector InitialCrouchOffset = FVector::ZeroVector;
	FVector TargetCrouchOffset = FVector::ZeroVector;
	float CrouchOffsetBlendPct = 1.0f;
	FVector CurrentCrouchOffset = FVector::ZeroVector;
};
