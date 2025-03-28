// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "K_YGTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UK_YGTimerWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    // 위젯이 생성될 때 호출되는 함수
    virtual void NativeConstruct() override;

public:
    // 카운트다운 시간 업데이트를 위한 함수
    void UpdateTimerDisplay(int32 CountdownTime);

    // 카운트다운 완료 후 호출되는 함수
    void CountdownHasFinished();

    // 5분 동안의 타이머 디스플레이를 업데이트하는 함수
    void Update5MinDisplay(int32 TimeInSeconds);

private:
    // 카운트다운 텍스트를 숨기는 함수
    void HideCountdownText();

    // 시간을 "MM:SS" 형식으로 포맷팅하는 함수
    FString FormatTime(int32 TimeInSeconds);

    // 텍스트를 숨기기 위한 타이머 핸들
    FTimerHandle HideTextTimerHandle;
};
