// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGSMG.h"

AK_YGSMG::AK_YGSMG()
{
	WeaponBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = WeaponBody;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/WeaponAssets/SMG/SM_SMG.SM_SMG"));

	if (SM_BODY.Succeeded()) {
		WeaponBody->SetStaticMesh(SM_BODY.Object);
	}
	 




	// 총 정보 구조체 초기화 
	WeaponInfo.WeaponKind = EWeapon::SMG;
	WeaponInfo.WeaponDamage = 10.0f;
	WeaponInfo.MagazinBulletCount = 50;
	WeaponInfo.CurrentUsableBulletCount = 50;
	WeaponInfo.MaxUsableBulletCount = 200;
	WeaponInfo.ReboundFoffeicient = 1.0f;
	WeaponInfo.DischargeMode = EWeaponDischargeMode::FULLAUTO;
	WeaponInfo.ReloadTime = 2.0f;
	WeaponInfo.CharacterSpeedRevicseWeight = 1.0f;
	WeaponInfo.NextBulletDischargeTime = 0.1;
	WeaponInfo.BulletVelocity = 50000.0f;   // 단위: m/s
	WeaponInfo.ZoomMagnificationMode = EZoomMagnificationMode::IRONSIGHT;

	InitMontage();
}

void AK_YGSMG::InitMontage()
{
	// Upper body idle animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYIDLE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Idle_SMG.AM_Idle_SMG"));
	if (UPPERBODYIDLE.Succeeded())
	{
		UpperBodyIdle = UPPERBODYIDLE.Object;
	}

	// Upper body fire animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYFIRE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Fire_SMG.AM_Fire_SMG"));
	if (UPPERBODYFIRE.Succeeded())
	{
		UpperBodyFire = UPPERBODYFIRE.Object;
	}

	// Upper body reload animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYRELOAD(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Reload_SMG.AM_Reload_SMG"));
	if (UPPERBODYRELOAD.Succeeded())
	{
		UpperBodyReload = UPPERBODYRELOAD.Object;
	}

	// Upper body Aiming animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYAIM(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Aim_SMG.AM_Aim_SMG"));
	if (UPPERBODYAIM.Succeeded())
	{
		UpperBodyAim = UPPERBODYAIM.Object;
	}
}
