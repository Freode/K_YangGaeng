// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/HealthBar/K_YGHealthBarManager.h"
#include "UI/HealthBar/HealthBarComponent.h"

// 생성자: 기본 체력 값과 컴포넌트 설정
AK_YGHealthBarManager::AK_YGHealthBarManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// HealthBarComponent를 생성하고 붙입니다.
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComponent"));
}

// 게임 시작시 호출: 컴포넌트를 초기화
void AK_YGHealthBarManager::BeginPlay()
{
	Super::BeginPlay();
}

// 매 프레임 호출되는 함수
void AK_YGHealthBarManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}