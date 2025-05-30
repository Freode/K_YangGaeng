// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "K_YGTimerManager.generated.h"

class UTextRenderComponent;
class UK_YGTimerWidget;

UCLASS()
class K_YANGGAENG_API AK_YGTimerManager : public AK_YGOnlyFightCharacter
{
	GENERATED_BODY()

public:
	// 기본 생성자
	AK_YGTimerManager();

protected:
	// 게임 시작 시 호출되는 함수
	virtual void BeginPlay() override;

public:
	// 매 프레임마다 호출되는 함수
	virtual void Tick(float DeltaTime) override;

private:
	// 타이머 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UTimerComponent* TimerComponent;
};
