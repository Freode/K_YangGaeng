#include "UI/WorldMap/K_YGWorldMapManager.h"
#include "UI/WorldMap/WorldMapComponent.h"

// 클래스 생성자
AK_YGWorldMapManager::AK_YGWorldMapManager()
{
    // WorldMapComponent를 생성하고 RootComponent에 부착합니다.
    WorldMapComponent = CreateDefaultSubobject<UWorldMapComponent>(TEXT("WorldMapComponent"));  
    WorldMapComponent->SetupAttachment(RootComponent);

}

void AK_YGWorldMapManager::BeginPlay()
{
    Super::BeginPlay();

}



