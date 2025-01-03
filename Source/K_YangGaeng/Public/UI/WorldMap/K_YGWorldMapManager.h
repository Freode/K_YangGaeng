// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "K_YGWorldMapManager.generated.h"

class UWorldMapComponent;

/**
 *
 */
UCLASS()
class K_YANGGAENG_API AK_YGWorldMapManager : public AK_YGOnlyFightCharacter
{
	GENERATED_BODY()

public:

	AK_YGWorldMapManager();

	virtual void BeginPlay() override;

private:
	// WorldMapComponent: 미니맵 기능을 담당하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UWorldMapComponent* WorldMapComponent;
};
