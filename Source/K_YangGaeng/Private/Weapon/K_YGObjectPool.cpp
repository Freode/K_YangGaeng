// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGObjectPool.h"
#include "Engine/World.h"




UK_YGObjectPool::UK_YGObjectPool()
{
	PoolSize = 30;
}

void UK_YGObjectPool::InitializePool(UWorld* World, TSubclassOf<AK_YGBulletBase> InBulletClass)
{
    // if world or bulletclass is not available, then return


    // save bullet class to create
    BulletClass = InBulletClass;
    // PoolSize 만큼 탄환 생성하여 풀에 추가
    for (int32 i = 0; i < PoolSize; ++i)
    {
        AK_YGBulletBase* Bullet = World->SpawnActor<AK_YGBulletBase>(BulletClass);

        // check if bullet is valid: if you don't, editor may be shut down.
        if (Bullet)
        {
            Bullet->Initialize();
            Bullet->Deactivate();
            BulletPool.Add(Bullet);

        }

    }
}


// Return to K_YGGun usable bullet 
AK_YGBulletBase* UK_YGObjectPool::GetBullet()
{
    for (AK_YGBulletBase* Bullet : BulletPool)
    {
        // find bullet not activate yet
        if (Bullet && !Bullet->bIsActive)
        {
            Bullet->Activate();
            return Bullet;
        }
    }

	return nullptr;
}

void UK_YGObjectPool::DeactivateAllBullets()
{
    for (AK_YGBulletBase* Bullet : BulletPool)
    {
        if (Bullet)
        {
            Bullet->Deactivate();
        }
    }
}

