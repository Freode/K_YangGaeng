// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "ShopComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class K_YANGGAENG_API UShopComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // 생성자: 초기 설정을 수행
    UShopComponent();

protected:
    // 게임이 시작될 때 호출되는 함수
    virtual void BeginPlay() override;

    // 플레이어 입력 컴포넌트를 설정하는 함수
    void SetupInputComponent();

private:
    // 상점 위젯 블루프린트 클래스
    TSubclassOf<UUserWidget> ShopWidgetClass;

    // 생성된 상점 위젯 인스턴스
    UUserWidget* ShopWidgetInstance;

    // 상점 위젯의 가시성 상태를 추적하는 변수
    bool bIsShopWidgetVisible;

    // 상점 위젯의 가시성을 토글하는 함수
    void ToggleShopWidget();

    // 마우스 커서의 상태를 토글하는 함수
    void ToggleMouseCursor(bool bShowCursor);
};
