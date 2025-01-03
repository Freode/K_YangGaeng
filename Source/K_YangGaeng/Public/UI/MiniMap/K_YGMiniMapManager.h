// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "K_YGMiniMapManager.generated.h"

class UMiniMapComponent;

/**
 *
 */
UCLASS()
class K_YANGGAENG_API AK_YGMiniMapManager : public AK_YGOnlyFightCharacter
{
	GENERATED_BODY()

public:
	// 클래스의 기본 생성자
	AK_YGMiniMapManager();

protected:
	// 게임 플레이가 시작될 때 호출
	virtual void BeginPlay() override;

private:
	// MiniMapComponent: 미니맵 기능을 담당하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UMiniMapComponent* MiniMapComponent;
};
