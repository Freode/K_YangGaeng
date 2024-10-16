// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "UObject/NoExportTypes.h"
#include "Weapon/K_YGBulletBase.h"
#include "K_YGObjectPool.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UK_YGObjectPool : public UObject
{
	GENERATED_BODY()

public:

	UK_YGObjectPool();

	// init pool
	void InitializePool(UWorld* World, TSubclassOf<AK_YGBulletBase> InBulletClass);

	// get Bullet Object
	AK_YGBulletBase* GetBullet();


	void DeactivateAllBullets();


private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AK_YGBulletBase> BulletClass;

	UPROPERTY(EditAnywhere)
	int32 PoolSize;

	TArray<AK_YGBulletBase*> BulletPool;

};
