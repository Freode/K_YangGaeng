// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGSmokeGrenade.h"

AK_YGSmokeGrenade::AK_YGSmokeGrenade()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = Body;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/SmokeGrenade/Meshs/SM_SmokeGrenade.SM_SmokeGrenade"));

	if (SM_BODY.Succeeded()) {
		Body->SetStaticMesh(SM_BODY.Object);
	}
}
