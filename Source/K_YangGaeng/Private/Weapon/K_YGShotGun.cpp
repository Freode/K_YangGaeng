// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGShotGun.h"

AK_YGShotGun::AK_YGShotGun()
{
	WeaponBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = WeaponBody;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/WeaponAssets/ShotGun/Meshs/SM_ShotGun.SM_ShotGun"));

	if (SM_BODY.Succeeded()) {
		WeaponBody->SetStaticMesh(SM_BODY.Object);
	}
	
	 


	// 총 정보 구조체 초기화 
	WeaponInfo.WeaponKind = EWeapon::SHOTGUN;
	WeaponInfo.WeaponDamage = 10.0f;
	WeaponInfo.MagazinBulletCount = 2;
	WeaponInfo.CurrentUsableBulletCount = 2;
	WeaponInfo.MaxUsableBulletCount = 50;
	WeaponInfo.ReboundFoffeicient = 1.0f;
	WeaponInfo.DischargeMode = EWeaponDischargeMode::SINGLESHOT;
	WeaponInfo.ReloadTime = 1.0f;
	WeaponInfo.CharacterSpeedRevicseWeight = 1.0f;
	WeaponInfo.NextBulletDischargeTime = 1.0;
	WeaponInfo.BulletVelocity = 50000.0f;   // 단위: m/s
	WeaponInfo.ZoomMagnificationMode = EZoomMagnificationMode::IRONSIGHT;

	InitMontage();
}

void AK_YGShotGun::InitMontage()
{
	// Upper body idle animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYIDLE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Idle_RifleIronsights.AM_Idle_RifleIronsights"));
	if (UPPERBODYIDLE.Succeeded())
	{
		UpperBodyIdle = UPPERBODYIDLE.Object;
	}

	// Upper body fire animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYFIRE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Fire_Shotgun_Ironsights.AM_Fire_Shotgun_Ironsights"));
	if (UPPERBODYFIRE.Succeeded())
	{
		UpperBodyFire = UPPERBODYFIRE.Object;
	}

	// Upper body reload animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYRELOAD(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Reload_Shotgun_Ironsights.AM_Reload_Shotgun_Ironsights"));
	if (UPPERBODYRELOAD.Succeeded())
	{
		UpperBodyReload = UPPERBODYRELOAD.Object;
	}
	else
	{
		K_YG_UELOG(Display, TEXT("Reload montage failed"));
	}

	// Upper body Aiming animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYAIM(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Aim_Shotgun_Ironsights.AM_Aim_Shotgun_Ironsights"));
	if (UPPERBODYAIM.Succeeded())
	{
		UpperBodyAim = UPPERBODYAIM.Object;
	}
}
