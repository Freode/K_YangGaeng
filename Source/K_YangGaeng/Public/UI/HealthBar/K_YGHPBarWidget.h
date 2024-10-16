// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "K_YGHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UK_YGHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 체력 퍼센트를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealthPercent(float Percent);

	// 체력 텍스트를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealthText(float CurrentHealth);

protected:
	// 첫 번째 체력 바
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_First;

	// 두 번째 체력 바
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Second;

	// 현재 체력을 표시하는 텍스트 블록
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthText;

private:
	// 두 번째 체력 바를 점진적으로 업데이트하는 함수
	void UpdateSecondProgressBar();

	// 타이머 핸들
	UPROPERTY()
	FTimerHandle TimerHandle;

	// 목표 퍼센트 값
	float TargetPercent;
};
