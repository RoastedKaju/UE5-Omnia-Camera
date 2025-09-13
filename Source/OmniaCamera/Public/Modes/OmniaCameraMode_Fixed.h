// Copyright (C) 2025 RoastedKaju. All Rights Reserved.

#pragma once

#include "OmniaCameraMode_Static.h"
#include "OmniaCameraMode_Fixed.generated.h"

class UObject;
class UOmniaCameraComponent;

/**
 * UOmniaCameraMode_TopDown
 *
 *	A basic third person camera mode that looks down at a fixed arena.
 */
UCLASS(Abstract, Blueprintable)
class OMNIACAMERA_API UOmniaCameraMode_Fixed : public UOmniaCameraMode_Static
{
	GENERATED_BODY()

public:
	UOmniaCameraMode_Fixed();

protected:
	//~UOmniaCameraMode interface
	virtual void OnActivation_Implementation() override;
	
	virtual void UpdateView(float DeltaTime) override;
	//~End of UOmniaCameraMode interface

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Omnia|Camera")
	bool bFocusOnPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Omnia|Camera", meta=(EditCondition = "bFocusOnPlayer", ClampMin="0.0", ClampMax="1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float CameraRotationLagSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Omnia|Camera")
	bool bUseCustomControlRotation;

private:
	FRotator PreviousDesiredRot;

	UPROPERTY()
	AActor* CameraOwner;
};

// TODO: Focus effects roll?
// TODO: Custom control rotation
// TODO: Ability to change target actor from owner to anything else

