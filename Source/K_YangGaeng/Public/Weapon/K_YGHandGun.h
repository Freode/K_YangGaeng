// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Weapon/K_YGGun.h"
#include "K_YGHandGun.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGHandGun : public AK_YGGun
{
	GENERATED_BODY()
	
public:
	AK_YGHandGun();

public:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

protected:
	void InitMontage();

};
