// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/Timer/K_YGTimerWidget.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h"

// 위젯이 생성될 때 호출되는 함수
void UK_YGTimerWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

// 카운트다운 타이머의 디스플레이를 업데이트
// CountdownTime을 텍스트로 설정
void UK_YGTimerWidget::UpdateTimerDisplay(int32 CountdownTime)
{
    // "CountdownText"라는 이름을 가진 UTextBlock 위젯 찾기
    UTextBlock* CountdownTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountdownText")));
    if (CountdownTextBlock)
    {
        // 카운트다운 시간을 텍스트로 설정
        CountdownTextBlock->SetText(FText::AsNumber(CountdownTime));
        UE_LOG(LogTemp, Warning, TEXT("K_YGTimerWidget: UpdateTimerDisplay called with %d"), CountdownTime);
    }

}

// 카운트다운이 완료된 후 호출되는 함수
void UK_YGTimerWidget::CountdownHasFinished()
{
    // "CountdownText"라는 이름을 가진 UTextBlock 위젯 찾기
    UTextBlock* CountdownTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountdownText")));
    if (CountdownTextBlock)
    {
        // 카운트다운이 끝났다는 것을 표시하기 위해 텍스트를 "GO!"로 변경
        CountdownTextBlock->SetText(FText::FromString(TEXT("GO!")));
        

        // 2초 후에 텍스트를 숨기기 위한 타이머 설정
        GetWorld()->GetTimerManager().SetTimer(HideTextTimerHandle, this, &UK_YGTimerWidget::HideCountdownText, 2.0f, false);
    }

}

// 타이머의 디스플레이를 업데이트
// TimeInSeconds를 포맷하여 "TimerText" 텍스트 블록에 설정
void UK_YGTimerWidget::Update5MinDisplay(int32 TimeInSeconds)
{
    // "TimerText"라는 이름을 가진 UTextBlock 위젯 찾기
    UTextBlock* TimerTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("TimerText")));
    if (TimerTextBlock)
    {
        // 남은 시간을 포맷하여 텍스트로 설정
        TimerTextBlock->SetText(FText::FromString(FormatTime(TimeInSeconds)));
        
    }
  
}

// 카운트다운 텍스트를 숨기는 함수
void UK_YGTimerWidget::HideCountdownText()
{
    // "CountdownText"라는 이름을 가진 UTextBlock 위젯 찾기
    UTextBlock* CountdownTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CountdownText")));
    if (CountdownTextBlock)
    {
        // 텍스트를 빈 문자열로 설정하여 텍스트 숨김
        CountdownTextBlock->SetText(FText::FromString(TEXT("")));
       
    }

}

// 초 단위 시간을 "MM:SS" 형식으로 포맷하는 함수
FString UK_YGTimerWidget::FormatTime(int32 TimeInSeconds)
{
    // 분과 초 계산
    int32 Minutes = TimeInSeconds / 60;
    int32 Seconds = TimeInSeconds % 60;

    // "MM:SS" 형식으로 문자열 반환
    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}
