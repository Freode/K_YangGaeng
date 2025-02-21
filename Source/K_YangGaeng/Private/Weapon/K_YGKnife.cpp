// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGKnife.h"

AK_YGKnife::AK_YGKnife()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = Body;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/FPS_Weapon_Bundle/Meshes/M9_Knife/SM_M9_Knife.SM_M9_Knife"));

	if (SM_BODY.Succeeded()) {
		Body->SetStaticMesh(SM_BODY.Object);
	}
}
