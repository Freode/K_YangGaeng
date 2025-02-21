// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGObjectPool.h"
#include "Engine/World.h"
#include <queue>



UK_YGObjectPool::UK_YGObjectPool()
{
	PoolSize = 30;
}

void UK_YGObjectPool::InitializePool(UWorld* World, TSubclassOf<AK_YGBulletBase> InBulletClass)
{ 
    if (!World || !*InBulletClass)
    {
        return;
    }
     
    BulletClass = InBulletClass;

    // create bullet object as 'PoolSize' and push in queue
    for (int32 i = 0; i < PoolSize; ++i)
    {
        AK_YGBulletBase* Bullet = World->SpawnActor<AK_YGBulletBase>(BulletClass);
         
        if (Bullet)
        {
            Bullet->Initialize();
            Bullet->Deactivate();
            BulletPoolQueue.push(Bullet); 
        }
    }
}

// return usable bullet from queue
AK_YGBulletBase* UK_YGObjectPool::GetBullet()
{
    if (!BulletPoolQueue.empty())
    {
        AK_YGBulletBase* Bullet = BulletPoolQueue.front();
        BulletPoolQueue.pop(); 

        if (Bullet && !Bullet->bIsActive)
        {
            Bullet->Activate();
            return Bullet;
        }
    }

    return nullptr;  
}

// return to pool that used bullet
void UK_YGObjectPool::ReturnBullet(AK_YGBulletBase* Bullet)
{
    if (Bullet)
    {
        Bullet->Deactivate();
        BulletPoolQueue.push(Bullet); 
    }
}

void UK_YGObjectPool::DeactivateAllBullets()
{
    std::queue<AK_YGBulletBase*> TempQueue;

    // deactivate all bullet object in queue
    while (!BulletPoolQueue.empty())
    {
        AK_YGBulletBase* Bullet = BulletPoolQueue.front();
        BulletPoolQueue.pop();

        if (Bullet)
        {
            Bullet->Deactivate();
        }

        TempQueue.push(Bullet);
    }

    // replace queue 
    BulletPoolQueue.swap(TempQueue);  
}

