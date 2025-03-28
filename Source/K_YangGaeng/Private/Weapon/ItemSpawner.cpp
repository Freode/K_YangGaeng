// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/ItemSpawner.h"
#include "Weapon/K_YGRifle.h"
#include "Weapon/K_YGHandGun.h"
#include "Weapon/K_YGSMG.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

    SetItemClass(AK_YGRifle::StaticClass());
    SpawnItem(FVector(0.0f, 0.0f, 50.0f), GetActorRotation());


    SetItemClass(AK_YGHandGun::StaticClass());
    SpawnItem(FVector(0.0f, 300.0f, 50.0f), GetActorRotation());

    SetItemClass(AK_YGSMG::StaticClass());
    SpawnItem(FVector(0.0f, 600, 50.0f), GetActorRotation());
	
}

void AItemSpawner::SpawnItem(FVector SpawnPosition, FRotator SpawnRotation)
{ 
    K_YG_SIMPLE(Warning);

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    // 스폰할 위치와 회전을 설정  

    // 아이템 스폰
    ABaseWeapon* SpawnedItem = GetWorld()->SpawnActor<ABaseWeapon>(ItemClass, SpawnPosition, SpawnRotation, SpawnParams);
    SpawnedItem->SetCollisionBlock();

} 

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

