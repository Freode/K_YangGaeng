// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/Timer/K_YGTimerManager.h"
#include "UI/Timer/TimerComponent.h"

// 기본 생성자
AK_YGTimerManager::AK_YGTimerManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// TimerComponent를 생성하고 붙이기
	TimerComponent = CreateDefaultSubobject<UTimerComponent>(TEXT("TimerComponent"));
}

// 게임 시작 시 호출되는 함수
void AK_YGTimerManager::BeginPlay()
{
	Super::BeginPlay();
}

// 매 프레임마다 호출되는 함수
void AK_YGTimerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}