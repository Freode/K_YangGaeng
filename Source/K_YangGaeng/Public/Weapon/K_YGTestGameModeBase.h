// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "GameFramework/GameModeBase.h"
#include "K_YGTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AK_YGTestGameModeBase();

	// Bullet Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	TSubclassOf<class AK_YGBulletBase> BulletClass;
};
