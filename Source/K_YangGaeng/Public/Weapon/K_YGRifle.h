// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Weapon/K_YGGun.h"
#include "K_YGRifle.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGRifle : public AK_YGGun
{
	GENERATED_BODY()

public:
	AK_YGRifle();


public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

protected:
	void InitMontage();

};
