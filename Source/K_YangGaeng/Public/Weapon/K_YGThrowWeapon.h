// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Weapon/BaseWeapon.h"
#include "K_YGThrowWeapon.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGThrowWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
	AK_YGThrowWeapon();

public:
	virtual void ThrowWeapon();
	virtual void Explosion();
	virtual void RemoveSafePin();

protected:
	struct FThrowWeaponInfo
	{
		float ThrowWeaponDamage;	// 데미지
		float DamageRange;			// 공격 범위
		int16 CurrentWeaponCount;	// 현재 소지 개수
		int16 MaxWeaponCount;		// 최대 소지 개수
	};

	enum class EThrowWeapon
	{
		EXPLOSIONGRENADE,			// 세열수류탄
		SMOKEGRENADE,				// 연막수류탄
		LIGHTGRENADE,				// 섬광수류탄
	};

	
};
