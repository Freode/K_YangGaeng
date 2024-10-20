// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Weapon/K_YGGun.h"
#include "K_YGSMG.generated.h"

/**
 * 
 */

UCLASS()
class K_YANGGAENG_API AK_YGSMG : public AK_YGGun
{
	GENERATED_BODY()

public:
	AK_YGSMG();

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

protected:
	void InitMontage();
};
