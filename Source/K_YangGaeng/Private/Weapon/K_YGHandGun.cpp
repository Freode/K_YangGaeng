// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGHandGun.h"

AK_YGHandGun::AK_YGHandGun()
{
	WeaponBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = WeaponBody;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/WeaponAssets/HandGun/Meshs/SM_Hand_Gun.SM_Hand_Gun"));

	if (SM_BODY.Succeeded()) {
		WeaponBody->SetStaticMesh(SM_BODY.Object);
	}
	 

	// 총 정보 구조체 초기화 
	WeaponInfo.WeaponKind = EWeapon::HANDGUN;
	WeaponInfo.WeaponDamage = 10.0f;
	WeaponInfo.MagazinBulletCount = 20;
	WeaponInfo.CurrentUsableBulletCount = 20;
	WeaponInfo.MaxUsableBulletCount = 500;
	WeaponInfo.ReboundFoffeicient = 1.0f;
	WeaponInfo.DischargeMode = EWeaponDischargeMode::SINGLESHOT;
	WeaponInfo.ReloadTime = 2.0f;
	WeaponInfo.CharacterSpeedRevicseWeight = 1.0f;
	WeaponInfo.NextBulletDischargeTime = 0.5;
	WeaponInfo.BulletVelocity = 50000.0f;   // 단위: m/s
	WeaponInfo.ZoomMagnificationMode = EZoomMagnificationMode::IRONSIGHT;

	InitMontage();
}

void AK_YGHandGun::InitMontage()
{
	// Upper body idle animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYIDLE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Idle_HandGun.AM_Idle_HandGun"));
	if (UPPERBODYIDLE.Succeeded())
	{
		UpperBodyIdle = UPPERBODYIDLE.Object;
	}

	// Upper body fire animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYFIRE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Fire_HandGun.AM_Fire_HandGun"));
	if (UPPERBODYFIRE.Succeeded())
	{
		UpperBodyFire = UPPERBODYFIRE.Object;
	}

	// Upper body reload animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYRELOAD(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Reload_HandGun.AM_Reload_HandGun"));
	if (UPPERBODYRELOAD.Succeeded())
	{
		UpperBodyReload = UPPERBODYRELOAD.Object;
	}

	// Upper body Aiming animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYAIM(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Aim_HandGun.AM_Aim_HandGun"));
	if (UPPERBODYAIM.Succeeded())
	{
		UpperBodyAim = UPPERBODYAIM.Object;
	}
}

