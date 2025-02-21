// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "K_YGCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UK_YGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	UK_YGCharacterMovementComponent();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", meta = (AllowPrivateAccess = true))
	FVector MaxVelocity;

};
