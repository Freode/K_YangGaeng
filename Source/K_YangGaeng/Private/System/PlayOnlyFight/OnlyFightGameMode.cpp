// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "System/PlayOnlyFight/OnlyFightGameMode.h"
#include "Character/K_YGOnlyFightCharacter.h"

AOnlyFightGameMode::AOnlyFightGameMode()
{
	DefaultPawnClass = AK_YGOnlyFightCharacter::StaticClass();
}