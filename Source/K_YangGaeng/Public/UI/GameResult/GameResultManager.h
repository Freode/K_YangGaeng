// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/K_YGTestOnlyFightCharacter.h"
#include "GameResultManager.generated.h"

/**
 *
 */

class UGameResultComponent;

UCLASS()
class K_YANGGAENG_API AGameResultManager : public AK_YGTestOnlyFightCharacter
{
	GENERATED_BODY()

public:
	AGameResultManager();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UGameResultComponent* GameResultComponent;
};
