// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Weapon/BaseWeapon.h"
#include "K_YGKnife.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGKnife : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AK_YGKnife();

protected:
	struct FKnifeInfo
	{
		float KnifeDamage;
		float KnifeDefense;
	};

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

};
