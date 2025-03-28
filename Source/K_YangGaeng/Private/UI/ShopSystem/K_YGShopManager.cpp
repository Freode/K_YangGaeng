// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/ShopSystem/K_YGShopManager.h"
#include "UI/ShopSystem/ShopComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// 생성자: 초기 설정 수행
AK_YGShopManager::AK_YGShopManager()
{
    // ShopComponent 생성 및 초기화
    ShopComponent = CreateDefaultSubobject<UShopComponent>(TEXT("ShopComponent"));
}

// 게임 시작 시 호출되는 함수
void AK_YGShopManager::BeginPlay()
{
    Super::BeginPlay();
    // ShopComponent의 BeginPlay가 호출되므로 별도 초기화 필요 없음
}

// 플레이어 입력 컴포넌트를 설정하는 함수
void AK_YGShopManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
}
