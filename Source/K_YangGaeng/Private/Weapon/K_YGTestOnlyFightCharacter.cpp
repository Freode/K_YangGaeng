// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGTestOnlyFightCharacter.h" 
#include "Weapon/WeaponActorComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/K_YGGun.h"
#include "Character/OnlyFightInputAComponent.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "Character/CameraActorComponent.h"


AK_YGTestOnlyFightCharacter::AK_YGTestOnlyFightCharacter()
{
	//USkeletalMeshComponent* SKMesh = GetMesh();

	//// use different AnimBP(that AnimBP_OnlyFight_Copy child made)
	//SKMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// 
	//// create component
	//WeaponComponent = CreateDefaultSubobject<UWeaponActorComponent>(TEXT("CreateComponent"));
	//static ConstructorHelpers::FClassFinder<UBaseCharacterAnimInstance> ANIMCLASS(TEXT("/Game/K_YangGaeng/Weapons/Animations/AnimStarterPack/AnimBP_OnlyFight_Copy.AnimBP_OnlyFight_Copy_C"));
	//if (ANIMCLASS.Succeeded())
	//{
	//	SKMesh->SetAnimInstanceClass(ANIMCLASS.Class);
	//	
	//}

}

void AK_YGTestOnlyFightCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);



}

void AK_YGTestOnlyFightCharacter::BeginPlay()
{
	Super::BeginPlay();

	//// bind input
	//WeaponComponent->BindInput(InputCustomComponent);
	//// create weapon object 
	//// set which gun will spawn
	//SpawnedGun = WeaponComponent->GetWeapon(EWeapon::SNIPERRIFLE);
	//SpawnedGun->SetCameraComponent(CameraComponent);

}
