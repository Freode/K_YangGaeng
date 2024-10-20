// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "GameFramework/GameModeBase.h"
#include "K_YG_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YG_LobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AK_YG_LobbyGameMode();

protected:
	virtual void BeginPlay() override;


};
