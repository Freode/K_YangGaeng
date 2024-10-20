// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/GameResult/GameResultComponent.h"
#include "Blueprint/UserWidget.h" // 위젯 관련 헤더 추가
#include "Kismet/GameplayStatics.h" // UGameplayStatics 사용을 위한 헤더

// Sets default values for this component's properties
UGameResultComponent::UGameResultComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GameResultWidget = nullptr; // 멤버 변수 초기화
}

// Called when the game starts
void UGameResultComponent::BeginPlay()
{
	Super::BeginPlay();

	// StaticLoadClass를 사용해 위젯 클래스를 런타임에 로드
	UClass* WidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("/Game/K_YangGaeng/UI/GameResult/WG_GameResult.WG_GameResult_C"));
	if (WidgetClass)
	{
		// 멤버 변수 GameResultWidget에 위젯을 생성하고 저장
		GameResultWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (GameResultWidget)
		{
			GameResultWidget->AddToViewport();
		}
	}
}

// Called every frame
void UGameResultComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 필요시 추가 로직 작성
}
