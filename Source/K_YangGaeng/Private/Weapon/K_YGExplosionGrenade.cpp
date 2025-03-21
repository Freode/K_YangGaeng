// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGExplosionGrenade.h"

AK_YGExplosionGrenade::AK_YGExplosionGrenade()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = Body;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/FPS_Weapon_Bundle/Meshes/G67_Grenade/SM_G67_X.SM_G67_X"));

	if (SM_BODY.Succeeded()) {
		Body->SetStaticMesh(SM_BODY.Object);
	}
}
