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

public:
    // 크레딧을 감소시키는 함수 선언
    bool DecreaseCredit(int32 Amount);
    // 플레이어 입력 컴포넌트를 설정하는 함수
    void SetupInputComponent(class UOnlyFightInputAComponent* InInputCustomComponent);

protected:
    // 게임이 시작될 때 호출되는 함수
    virtual void BeginPlay() override;

    class AK_YGOnlyFightCharacter* K_YGCharacter;


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
    // 현재 가지고 있는 크레딧
    int32 CurrentCredit = 5000;

    // 크레딧을 표시하는 텍스트를 업데이트하는 함수
    void UpdateCreditText();

    // 버튼 클릭 이벤트를 바인딩하는 함수
    void BindButtonEvents();



    // 버튼 클릭 시 호출될 함수
    UFUNCTION()
    void OnFragmentationGrenadeButtonClicked();

    UFUNCTION()
    void OnSmokeGrenadeButtonClicked();

    UFUNCTION()
    void OnFlashGrenadeButtonClicked();

    UFUNCTION()
    void OnSMGButtonClicked();

    UFUNCTION()
    void OnShotGunButton1Clicked();

    UFUNCTION()
    void OnShotGunButton2Clicked();

    UFUNCTION()
    void OnRipleButton1Clicked();

    UFUNCTION()
    void OnRipleButton2Clicked();

    UFUNCTION()
    void OnSniperButtonClicked();
    UFUNCTION()
    void OnHandGunButtonClicked();

// 무기 이름 및 가격
private:
    FString FragmentationGrenadeName = TEXT("Fragmentation\nGrenade");
    FString SmokeGrenadeName = TEXT("Smoke Grenade");
    FString FlashGrenadeName = TEXT("Flash Grenade");
    FString SMGName = TEXT("Submachine Gun");
    FString ShotGun1Name = TEXT("Shotgun Model 1");
    FString ShotGun2Name = TEXT("Shotgun Model 2");
    FString Riple1Name = TEXT("Rifle Model 1");
    FString Riple2Name = TEXT("Rifle Model 2");
    FString SniperName = TEXT("Sniper Rifle");
    FString HandGunName = TEXT("Hand Gun");

    int32 FragmentationGrenadePrice = 500;
    int32 SmokeGrenadePrice = 500;
    int32 FlashGrenadePrice = 500;
    int32 HandGunPrice = 500;
    int32 SMGPrice = 1000;
    int32 ShotGun1Price = 1500;
    int32 ShotGun2Price = 1700;
    int32 Riple1Price = 2000;
    int32 Riple2Price = 2200;
    int32 SniperPrice = 2500;
};
