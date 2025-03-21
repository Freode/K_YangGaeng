// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGSniperRifle.h"

AK_YGSniperRifle::AK_YGSniperRifle()
{
	WeaponBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPONBODY"));

	RootComponent = WeaponBody;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/WeaponAssets/SniperRifle/SM_SniperRifle.SM_SniperRifle"));

	if (SM_BODY.Succeeded()) {
		WeaponBody->SetStaticMesh(SM_BODY.Object);
	}
	 

	// 총 정보 구조체 초기화 
	WeaponInfo.WeaponKind = EWeapon::SNIPERRIFLE;
	WeaponInfo.WeaponDamage = 10.0f;
	WeaponInfo.MagazinBulletCount = 20;
	WeaponInfo.CurrentUsableBulletCount = 20;
	WeaponInfo.MaxUsableBulletCount = 100;
	WeaponInfo.ReboundFoffeicient = 1.0f;
	WeaponInfo.DischargeMode = EWeaponDischargeMode::BOLTACTION;
	WeaponInfo.ReloadTime = 2.0f;
	WeaponInfo.CharacterSpeedRevicseWeight = 1.0f;
	WeaponInfo.NextBulletDischargeTime = 1.0;
	WeaponInfo.BulletVelocity =100000.0;   // 단위: m/s
	WeaponInfo.ZoomMagnificationMode = EZoomMagnificationMode::IRONSIGHT;

	InitMontage();
}

void AK_YGSniperRifle::InitMontage()
{
	// Upper body idle animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYIDLE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Idle_RifleIronSights.AM_Idle_RifleIronSights"));
	if (UPPERBODYIDLE.Succeeded())
	{
		UpperBodyIdle = UPPERBODYIDLE.Object;
	}

	// Upper body fire animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYFIRE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Fire_Rifle_Ironsights.AM_Fire_Rifle_Ironsights"));
	if (UPPERBODYFIRE.Succeeded())
	{
		UpperBodyFire = UPPERBODYFIRE.Object;
	}

	// Upper body reload animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYRELOAD(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Reload_Rifle_Ironsights.AM_Reload_Rifle_Ironsights"));
	if (UPPERBODYRELOAD.Succeeded())
	{
		UpperBodyReload = UPPERBODYRELOAD.Object;
	}

	// Upper body Aiming animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYAIM(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Aim_RifleIronsights.AM_Aim_RifleIronsights"));
	if (UPPERBODYAIM.Succeeded())
	{
		UpperBodyAim = UPPERBODYAIM.Object;
	}
}
