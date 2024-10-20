// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/GameResult/GameResultManager.h"
#include "UI/GameResult/GameResultComponent.h"

// Sets default values
AGameResultManager::AGameResultManager()
{
	// GameResultComponent를 생성하고 이 액터에 첨부
	GameResultComponent = CreateDefaultSubobject<UGameResultComponent>(TEXT("GameResultComponent"));

	// SetupAttachment는 사용하지 않음, 대신 액터에 컴포넌트를 추가
	AddInstanceComponent(GameResultComponent);
}
