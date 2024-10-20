// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "K_YGShopManager.generated.h"

class UShopComponent;

/**
 *
 */
UCLASS()
class K_YANGGAENG_API AK_YGShopManager : public AK_YGOnlyFightCharacter
{
    GENERATED_BODY()

public:
    // 생성자: 초기 설정을 수행
    AK_YGShopManager();

protected:
    // 게임이 시작될 때 호출되는 함수
    virtual void BeginPlay() override;

    // 플레이어 입력 컴포넌트를 설정하는 함수
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // 상점 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UShopComponent* ShopComponent;
};