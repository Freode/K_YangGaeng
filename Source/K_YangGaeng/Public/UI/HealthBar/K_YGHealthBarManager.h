// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "K_YGHealthBarManager.generated.h"

/**
 *
 */
UCLASS()
class K_YANGGAENG_API AK_YGHealthBarManager : public AK_YGOnlyFightCharacter
{
	GENERATED_BODY()

public:
	// 생성자: 기본 체력 값과 컴포넌트 설정
	AK_YGHealthBarManager();

protected:
	// 게임 시작시 호출: 컴포넌트를 초기화
	virtual void BeginPlay() override;

public:
	// 매 프레임 호출되는 함수
	virtual void Tick(float DeltaTime) override;

//private:
	// 체력 바 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UHealthBarComponent* HealthBarComponent;
};