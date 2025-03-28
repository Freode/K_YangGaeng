// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/HealthBar/K_YGHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Engine/World.h"

// 체력 퍼센트를 설정하는 함수:
// 첫 번째 체력 바는 즉시 업데이트
// 두 번째 체력 바는 점진적으로 업데이트
void UK_YGHPBarWidget::SetHealthPercent(float Percent)
{
	if (ProgressBar_First)
	{
		ProgressBar_First->SetPercent(Percent);
	}

	if (ProgressBar_Second)
	{
		// 타겟 퍼센트를 설정
		TargetPercent = Percent; 

		// ProgressBar_Second의 점진적 감소를 시작
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UK_YGHPBarWidget::UpdateSecondProgressBar, 0.05f, true);
	}
}

// 체력 텍스트를 설정하는 함수: 현재 체력을 텍스트로 표시
void UK_YGHPBarWidget::SetHealthText(float CurrentHealth)
{
	if (CurrentHealthText)
	{
		CurrentHealthText->SetText(FText::AsNumber(CurrentHealth));
	}
}

// 두 번째 체력 바를 점진적으로 업데이트하는 함수
void UK_YGHPBarWidget::UpdateSecondProgressBar()
{
	if (!ProgressBar_Second)
	{
		return;
	}

	float CurrentPercent = ProgressBar_Second->GetPercent();
	if (CurrentPercent > TargetPercent)
	{
		// ProgressBar를 0.5초 동안 점진적으로 감소 (0.05초 간격으로 10단계)
		float NewPercent = FMath::FInterpTo(CurrentPercent, TargetPercent, 0.05f, 10.0f); // 속도 인자 10.0f
		ProgressBar_Second->SetPercent(NewPercent);

		// 목표에 도달하면 타이머 중지
		if (FMath::IsNearlyEqual(NewPercent, TargetPercent, 0.01f))
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}
