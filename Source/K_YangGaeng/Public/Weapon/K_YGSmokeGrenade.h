// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Weapon/K_YGThrowWeapon.h"
#include "K_YGSmokeGrenade.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGSmokeGrenade : public AK_YGThrowWeapon
{
	GENERATED_BODY()

public:
	AK_YGSmokeGrenade();

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

};
