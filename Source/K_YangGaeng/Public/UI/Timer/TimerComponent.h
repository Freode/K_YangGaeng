// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UTimerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTimerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 카운트다운 디스플레이를 업데이트하는 함수
	void UpdateTimerDisplay();

	// 타이머를 1초 단위로 감소시키는 함수
	void AdvanceTimer();

	// 카운트다운이 완료된 후 호출되는 함수
	void CountdownHasFinished();

	// 5분 타이머의 디스플레이를 업데이트하는 함수
	void Update5MinDisplay();

	// 카운트다운 위젯의 클래스 참조
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UK_YGTimerWidget> CountdownWidgetClass;

	// 생성된 카운트다운 위젯의 인스턴스
	UK_YGTimerWidget* CountdownWidget;

	// 카운트다운 타이머의 핸들
	FTimerHandle CountdownTimerHandle;

	// 5분 타이머의 핸들
	FTimerHandle FiveMinTimerHandle;

	// 카운트다운 시간 (초 단위)
	int32 CountdownTime;

	// 5분 타이머 시간 (초 단위)
	int32 FiveMinTime;
		
};
