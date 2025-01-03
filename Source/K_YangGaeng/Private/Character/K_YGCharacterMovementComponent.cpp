// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Character/K_YGCharacterMovementComponent.h"

UK_YGCharacterMovementComponent::UK_YGCharacterMovementComponent()
{
	// === Set character's move direction control ===

	// Use rotation based on move direction
	bOrientRotationToMovement = false;

	// Rotation velocity
	RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// Set max velocity
	MaxVelocity = FVector(300.0f, 300.0f, 300.0f);
}