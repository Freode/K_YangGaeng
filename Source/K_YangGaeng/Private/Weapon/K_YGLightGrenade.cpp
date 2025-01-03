// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGLightGrenade.h"

AK_YGLightGrenade::AK_YGLightGrenade()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = Body;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/LightGrenade/uploads_files_3308090_Flash+Grenade.uploads_files_3308090_Flash+Grenade"));

	if (SM_BODY.Succeeded()) {
		Body->SetStaticMesh(SM_BODY.Object);
	}
}
