// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/ShopSystem/ShopComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"

// 생성자: 초기 설정 수행
UShopComponent::UShopComponent()
    : bIsShopWidgetVisible(false)
{
    PrimaryComponentTick.bCanEverTick = false;

    // 상점 블루프린트 위젯 찾기
    static ConstructorHelpers::FClassFinder<UUserWidget> ShopWidgetBPClass(TEXT("/Game/K_YangGaeng/UI/ShopSystem/WG_ShopBase.WG_ShopBase_C"));
    if (ShopWidgetBPClass.Class != nullptr)
    {
        // 위젯 클래스 설정
        ShopWidgetClass = ShopWidgetBPClass.Class;
    }
}

// 게임 시작 시 호출되는 함수
void UShopComponent::BeginPlay()
{
    Super::BeginPlay();

    // 위젯을 생성하고 뷰포트에 추가
    if (ShopWidgetClass != nullptr)
    {
        // 위젯 인스턴스 생성
        ShopWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), ShopWidgetClass);
        if (ShopWidgetInstance != nullptr)
        {
            // 위젯을 뷰포트에 추가
            ShopWidgetInstance->AddToViewport();
            // 시작 시에는 위젯을 숨김
            ShopWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // 입력 컴포넌트를 설정
    SetupInputComponent();
}

// 플레이어 입력 컴포넌트를 설정하는 함수
void UShopComponent::SetupInputComponent()
{
    AActor* Owner = GetOwner();
    if (Owner != nullptr)
    {
        UInputComponent* InputComponent = Owner->FindComponentByClass<UInputComponent>();
        if (InputComponent != nullptr)
        {
            // 'B' 키를 입력으로 받아들이도록 처리
            InputComponent->BindAction("OpenShop", IE_Pressed, this, &UShopComponent::ToggleShopWidget);
        }
    }
}

// 상점 위젯의 가시성을 토글하는 함수
void UShopComponent::ToggleShopWidget()
{
    if (ShopWidgetInstance != nullptr)
    {
        // 현재 상태를 토글
        bIsShopWidgetVisible = !bIsShopWidgetVisible;

        if (bIsShopWidgetVisible)
        {
            // 위젯을 표시
            ShopWidgetInstance->SetVisibility(ESlateVisibility::Visible);
            // 마우스 커서 활성화
            ToggleMouseCursor(true);
        }
        else
        {
            // 위젯을 숨김
            ShopWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
            // 마우스 커서 비활성화
            ToggleMouseCursor(false);
        }
    }
}

// 마우스 커서의 상태를 토글하는 함수
void UShopComponent::ToggleMouseCursor(bool bShowCursor)
{
     // 첫 번째 플레이어 컨트롤러 가져오기
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController != nullptr)
    {
        // 마우스 커서 표시/숨김 설정
        // 클릭 이벤트 활성화/비활성화
        // 마우스 오버 이벤트 활성화/비활성화
        PlayerController->bShowMouseCursor = bShowCursor;
        PlayerController->bEnableClickEvents = bShowCursor;
        PlayerController->bEnableMouseOverEvents = bShowCursor;

        if (bShowCursor)
        {
            // 게임 및 UI에서 모두 입력을 받아들이도록 설정
            // 마우스를 뷰포트에 고정하지 않음
            // 포커스를 상점 위젯으로 설정
            // 입력 모드 설정 적용
            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputMode.SetWidgetToFocus(ShopWidgetInstance->TakeWidget());
            PlayerController->SetInputMode(InputMode);
        }
        else
        {
            // 게임 전용 입력 모드 설정
            PlayerController->SetInputMode(FInputModeGameOnly());
        }
    }
}
