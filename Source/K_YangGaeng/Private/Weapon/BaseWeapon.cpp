// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/BaseWeapon.h"
#include "Weapon/K_YGTestOnlyFightCharacter.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// get collision component
	UCapsuleComponent* WeaponCapsule = FindComponentByClass<UCapsuleComponent>();

	if (WeaponCapsule)
	{ 
		WeaponCapsule->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
		WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		WeaponCapsule->SetCollisionResponseToAllChannels(ECR_Overlap);
	} 



	// Upper body idle animation asset
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UPPERBODYIDLE(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/Montages/AM_Idle.AM_Idle"));
	if (UPPERBODYIDLE.Succeeded())
	{
		UpperBodyOriginalIdle = UPPERBODYIDLE.Object;

	}




}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());

}

void ABaseWeapon::CheckPlayerCharacter()
{
	if (nullptr == Character)
	{
		Character = Cast<ACharacter>(GetOwner());
	}

}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::EWeaponToString(EWeapon WeaponKind)
{
	switch (WeaponKind)
	{
	case EWeapon::NONE:
		K_YG_UELOG(Display, TEXT("None"));
		break;
	case EWeapon::HANDGUN:
		K_YG_UELOG(Display, TEXT("Hand Gun"));
		break;

	case EWeapon::RIFLE:
		K_YG_UELOG(Display, TEXT("Rifle"));
		break;

	case EWeapon::SNIPERRIFLE:
		K_YG_UELOG(Display, TEXT("Sniper Rifle"));
		break;

	case EWeapon::SMG:
		K_YG_UELOG(Display, TEXT("SMG"));
		break;

	case EWeapon::SHOTGUN:
		K_YG_UELOG(Display, TEXT("Shot Gun"));
		break;

	default:
		K_YG_UELOG(Display, TEXT("ect."));
		break;
	}
}


void ABaseWeapon::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && UpperBodyIdle)
		{
			// Idle 몽타주 실행
			AnimInstance->Montage_Play(UpperBodyIdle);
		}
	}
}

void ABaseWeapon::SetCollisionBlock()
{
	K_YG_CHECK(WeaponBody);
	WeaponBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void ABaseWeapon::SetCollisionOverlap()
{
	K_YG_CHECK(WeaponBody);
	WeaponBody->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

