// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGTestGameModeBase.h"
#include "Weapon/K_YGTestOnlyFightCharacter.h"
#include "Weapon/K_YGGun.h"

AK_YGTestGameModeBase::AK_YGTestGameModeBase()
{
	DefaultPawnClass = AK_YGTestOnlyFightCharacter::StaticClass();

}
