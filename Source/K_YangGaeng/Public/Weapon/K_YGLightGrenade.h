// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Weapon/K_YGThrowWeapon.h"
#include "K_YGLightGrenade.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGLightGrenade : public AK_YGThrowWeapon
{
	GENERATED_BODY()

public:
	AK_YGLightGrenade();

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	
};
