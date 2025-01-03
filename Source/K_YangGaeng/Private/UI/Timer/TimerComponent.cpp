// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/Timer/TimerComponent.h"
#include "UI/Timer/K_YGTimerWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

// 컴포넌트의 기본 값을 설정하는 생성자
UTimerComponent::UTimerComponent()
{
	// 매 프레임마다 Tick 함수가 호출되도록 설정
	PrimaryComponentTick.bCanEverTick = true; 
	// 초기 카운트다운 시간 설정
	CountdownTime = 3; 

	// 초기 타이머 시간 설정 
	FiveMinTime = 300; 

	// 위젯 블루프린트 클래스를 지정된 경로에서 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> CountdownWidgetBPClass(TEXT("/Game/K_YangGaeng/UI/Timer/WG_Timer.WG_Timer_C"));
	if (CountdownWidgetBPClass.Class != nullptr)
	{
		// CountdownWidgetClass에 로드한 위젯 클래스 설정
		CountdownWidgetClass = CountdownWidgetBPClass.Class; 
	}
}

// 컴포넌트가 게임 시작 시 호출되는 함수
void UTimerComponent::BeginPlay()
{
	// 부모 클래스의 BeginPlay 함수 호출
	Super::BeginPlay(); 

	// 위젯 클래스가 유효한지 확인
	if (CountdownWidgetClass != nullptr) 

	{
		// 위젯 인스턴스 생성
		CountdownWidget = CreateWidget<UK_YGTimerWidget>(GetWorld(), CountdownWidgetClass); 
		// 위젯 인스턴스가 유효한지 확인
		if (CountdownWidget != nullptr) 
		{
			// 위젯을 뷰포트에 추가하여 화면에 표시
			CountdownWidget->AddToViewport(); 
			// 초기 카운트다운 시간으로 타이머 디스플레이 업데이트
			UpdateTimerDisplay(); 
			GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UTimerComponent::AdvanceTimer, 1.0f, true); // 매초마다 AdvanceTimer 함수를 호출하는 타이머 설정
		}
	}
}

// 매 프레임마다 호출되는 함수
void UTimerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// 부모 클래스의 TickComponent 함수 호출
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 
}

// 카운트다운 타이머의 디스플레이를 업데이트하는 함수
void UTimerComponent::UpdateTimerDisplay()
{
	// 위젯 인스턴스가 유효한지 확인
	if (CountdownWidget != nullptr) 
	{
		// 남은 시간을 위젯에 업데이트
		CountdownWidget->UpdateTimerDisplay(FMath::Max(CountdownTime, 0)); 
	}
}

// 매초 호출되어 카운트다운을 진행하는 함수
void UTimerComponent::AdvanceTimer()
{
	// 카운트다운 시간 1초 감소
	--CountdownTime; 
	// 새로운 카운트다운 시간으로 디스플레이 업데이트
	UpdateTimerDisplay(); 

	// 카운트다운이 완료되었는지 확인
	if (CountdownTime < 1) 
	{
		// 카운트다운 타이머를 중지
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle); 
		// 카운트다운 완료 후 호출할 함수 호출
		CountdownHasFinished();
	}
}

// 카운트다운 타이머가 완료된 후 수행할 작업을 정의하는 함수
void UTimerComponent::CountdownHasFinished()
{
	// 위젯 인스턴스가 유효한지 확인
	if (CountdownWidget != nullptr)
	{
		// 카운트다운 완료 상태를 위젯에 업데이트
		CountdownWidget->CountdownHasFinished(); 
		// 타이머를 시작하여 매초마다 Update5MinDisplay 함수를 호출하는 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(FiveMinTimerHandle, this, &UTimerComponent::Update5MinDisplay, 1.0f, true); 
	}
}

// 타이머의 디스플레이를 업데이트하는 함수
void UTimerComponent::Update5MinDisplay()
{
	// 타이머 시간 1초 감소
	--FiveMinTime;

	// 위젯 인스턴스가 유효한지 확인
	if (CountdownWidget != nullptr) 
	{
		// 남은 시간으로 디스플레이 업데이트
		CountdownWidget->Update5MinDisplay(FMath::Max(FiveMinTime, 0)); 
	}
	// 타이머가 완료되었는지 확인
	if (FiveMinTime < 1) 
	{
		// 타이머를 중지
		GetWorld()->GetTimerManager().ClearTimer(FiveMinTimerHandle); 
	}
}
