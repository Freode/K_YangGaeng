// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/MiniMap/K_YGMiniMapManager.h"
#include "UI/MiniMap/MiniMapComponent.h"

// 클래스 생성자
AK_YGMiniMapManager::AK_YGMiniMapManager()
{
    // MiniMapComponent를 생성하고 소유자에 부착
    MiniMapComponent = CreateDefaultSubobject<UMiniMapComponent>(TEXT("MiniMapComponent"));
    MiniMapComponent->SetupAttachment(RootComponent);
}

void AK_YGMiniMapManager::BeginPlay()
{
    Super::BeginPlay();
}
