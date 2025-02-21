// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGRifle.h"

AK_YGRifle::AK_YGRifle()
{
	// 스태틱 메시 설정
	WeaponBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = WeaponBody;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/WeaponAssets/FPS_Weapon_Bundle/Meshes/AR4/SM_AR4_X.SM_AR4_X"));

	if (SM_BODY.Succeeded()) {
		WeaponBody->SetStaticMesh(SM_BODY.Object);
	}
	 

	// 총 정보 구조체 초기화 
	WeaponInfo.WeaponKind = EWeapon::RIFLE;
	WeaponInfo.WeaponDamage = 10.0f;
	WeaponInfo.MagazinBulletCount = 30;
	WeaponInfo.CurrentUsableBulletCount = 30;
	WeaponInfo.MaxUsableBulletCount = 50;
	WeaponInfo.ReboundFoffeicient = 1.0f;
	WeaponInfo.DischargeMode = EWeaponDischargeMode::ALL;
	WeaponInfo.ReloadTime = 2.0f;
	WeaponInfo.CharacterSpeedRevicseWeight = 1.0f;
	WeaponInfo.NextBulletDischargeTime = 0.2f;
	WeaponInfo.BulletVelocity = 50000.0f;   
	WeaponInfo.ZoomMagnificationMode = EZoomMagnificationMode::IRONSIGHT;

	InitMontage();
}

void AK_YGRifle::InitMontage()
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
