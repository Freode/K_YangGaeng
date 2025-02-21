// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/ShopSystem/ShopComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h" 
#include "Character/OnlyFightInputAComponent.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "Weapon/BaseWeapon.h"

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
            // 다른 위젯에 가려져 마우스 입력 무시되는 현상 해소를 위해 z order를 10으로 설정 
            ShopWidgetInstance->AddToViewport(10);
            // 시작 시에는 위젯을 숨김
            ShopWidgetInstance->SetVisibility(ESlateVisibility::Hidden);

            // 초기 크레딧 텍스트 업데이트
            UpdateCreditText();

            // 버튼 이벤트 바인딩
            BindButtonEvents();
        }
    }

    K_YGCharacter = Cast<AK_YGOnlyFightCharacter>(GetOwner());

    // 입력 컴포넌트를 설정
    //SetupInputComponent();
}

bool UShopComponent::DecreaseCredit(int32 Amount)
{
    // Amount가 현재 크레딧보다 크면 아무 작업도 하지 않음
    if (Amount > CurrentCredit)
    {
        UE_LOG(LogTemp, Warning, TEXT("크레딧 감소량이 현재 크레딧보다 큽니다. 크레딧이 줄어들지 않습니다. 현재 크레딧: %d, 감소량: %d"), CurrentCredit, Amount);
        return false; // 함수 종료
    }

    // Amount만큼 CurrentCredit 감소, 크레딧이 0보다 작아지지 않도록 설정
    CurrentCredit -= Amount;

    // 크레딧이 변경될 때마다 텍스트를 업데이트
    UpdateCreditText();
    return true;
}

// 플레이어 입력 컴포넌트를 설정하는 함수
void UShopComponent::SetupInputComponent(UOnlyFightInputAComponent* InInputCustomComponent)
{ 
    if (InInputCustomComponent)
    {
        // 'B' 키를 입력으로 받아들이도록 처리 
        InInputCustomComponent->OnInputShop.BindUObject(this, &UShopComponent::ToggleShopWidget);
    }
}

// 상점 위젯의 가시성을 토글하는 함수
void UShopComponent::ToggleShopWidget()
{
    if (ShopWidgetInstance != nullptr)
    {
        // 현재 상태를 토글
        bIsShopWidgetVisible = !bIsShopWidgetVisible; 
        K_YGCharacter->bIsWidgetActivated = !K_YGCharacter->bIsWidgetActivated;

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
            // 게임과 UI에서 모두 입력을 받아들이도록 설정
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

// 크레딧 텍스트를 업데이트하는 함수
void UShopComponent::UpdateCreditText()
{
    if (ShopWidgetInstance != nullptr)
    {
        // CurrentCreditText 텍스트 블록을 찾음
        if (UTextBlock* CreditTextBlock = Cast<UTextBlock>(ShopWidgetInstance->GetWidgetFromName(TEXT("CurrentCreditText"))))
        {
            // 현재 크레딧을 텍스트 형식으로 설정
            FString CreditString = FString::Printf(TEXT("CREDIT : %d$"), CurrentCredit);
            CreditTextBlock->SetText(FText::FromString(CreditString));
        }
    }
}

void UShopComponent::BindButtonEvents()
{
    if (ShopWidgetInstance != nullptr)
    {
        // 무기 정보를 배열로 구성하여 쉽게 설정
        struct FWeaponInfo
        {
            FString ButtonName;
            FString WeaponName;
            int32 WeaponPrice;
        };

        TArray<FWeaponInfo> WeaponInfoArray = {
            {TEXT("FragmentationGrenadeButton"), FragmentationGrenadeName, FragmentationGrenadePrice},
            {TEXT("SmokeGrenadeButton"), SmokeGrenadeName, SmokeGrenadePrice},
            {TEXT("FlashGrenadeButton"), FlashGrenadeName, FlashGrenadePrice},
            {TEXT("SMGButton"), SMGName, SMGPrice},
            {TEXT("ShotGunButton1"), ShotGun1Name, ShotGun1Price},
            {TEXT("ShotGunButton2"), ShotGun2Name, ShotGun2Price},
            {TEXT("RipleButton1"), Riple1Name, Riple1Price},
            {TEXT("RipleButton2"), Riple2Name, Riple2Price},
            {TEXT("SniperButton"), SniperName, SniperPrice},
            {TEXT("HandGunButton"), HandGunName, HandGunPrice }
        };

        for (const FWeaponInfo& WeaponInfo : WeaponInfoArray)
        {
            // 버튼 위젯을 찾음
            if (UUserWidget* WeaponWidget = Cast<UUserWidget>(ShopWidgetInstance->GetWidgetFromName(*WeaponInfo.ButtonName)))
            {
                // 버튼 클릭 이벤트 바인딩
                if (UButton* InnerButton = Cast<UButton>(WeaponWidget->GetWidgetFromName(TEXT("Button"))))
                {
                    if (WeaponInfo.ButtonName == TEXT("FragmentationGrenadeButton"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnFragmentationGrenadeButtonClicked);
                    else if (WeaponInfo.ButtonName == TEXT("SmokeGrenadeButton"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnSmokeGrenadeButtonClicked);
                    else if (WeaponInfo.ButtonName == TEXT("FlashGrenadeButton"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnFlashGrenadeButtonClicked);
                    else if (WeaponInfo.ButtonName == TEXT("SMGButton"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnSMGButtonClicked);
                    else if (WeaponInfo.ButtonName == TEXT("ShotGunButton1"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnShotGunButton1Clicked);
                    else if (WeaponInfo.ButtonName == TEXT("ShotGunButton2"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnShotGunButton2Clicked);
                    else if (WeaponInfo.ButtonName == TEXT("RipleButton1"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnRipleButton1Clicked);
                    else if (WeaponInfo.ButtonName == TEXT("RipleButton2"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnRipleButton2Clicked);
                    else if (WeaponInfo.ButtonName == TEXT("SniperButton"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnSniperButtonClicked);
                    else if (WeaponInfo.ButtonName == TEXT("HandGunButton"))
                        InnerButton->OnClicked.AddDynamic(this, &UShopComponent::OnHandGunButtonClicked);
                }

                // WeaponName 텍스트 블록을 설정
                if (UTextBlock* NameTextBlock = Cast<UTextBlock>(WeaponWidget->GetWidgetFromName(TEXT("WeaponName"))))
                {
                    NameTextBlock->SetText(FText::FromString(WeaponInfo.WeaponName));
                }

                // WeaponPrice 텍스트 블록을 설정
                if (UTextBlock* PriceTextBlock = Cast<UTextBlock>(WeaponWidget->GetWidgetFromName(TEXT("WeaponPrice"))))
                {
                    FString PriceString = FString::Printf(TEXT("%d$"), WeaponInfo.WeaponPrice);
                    PriceTextBlock->SetText(FText::FromString(PriceString));
                }
            }
        }
    }
}




void UShopComponent::OnFragmentationGrenadeButtonClicked()
{
    // if credit is enough (DecreaseCredit() is return true)
    // get weapon 
    if (DecreaseCredit(FragmentationGrenadePrice))
    {
        // when player's weapon list is pull
        if (!(K_YGCharacter->PutWepaon(EWeapon::SMG)))
        {
            // restore credit
            DecreaseCredit(-1 * FragmentationGrenadePrice);
        }
    }

}

void UShopComponent::OnSmokeGrenadeButtonClicked()
{
    DecreaseCredit(SmokeGrenadePrice); 
   
}

void UShopComponent::OnFlashGrenadeButtonClicked()
{
    DecreaseCredit(FlashGrenadePrice); 

}

void UShopComponent::OnSMGButtonClicked()
{    
    // if credit is enough (DecreaseCredit() is return true)
    // get weapon 
    if (DecreaseCredit(FragmentationGrenadePrice))
    {
        // when player's weapon list is pull
        if (!(K_YGCharacter->PutWepaon(EWeapon::SMG)))
        {
            // restore credit
            DecreaseCredit(-1 * FragmentationGrenadePrice);
        }
    }

}

void UShopComponent::OnShotGunButton1Clicked()
{
    // if credit is enough (DecreaseCredit() is return true)
    // get weapon 
    if (DecreaseCredit(FragmentationGrenadePrice))
    {
        // when player's weapon list is pull
        if (!(K_YGCharacter->PutWepaon(EWeapon::SHOTGUN)))
        {
            // restore credit
            DecreaseCredit(-1 * FragmentationGrenadePrice);
        }
    }
}

void UShopComponent::OnShotGunButton2Clicked()
{
    // if credit is enough (DecreaseCredit() is return true)
    // get weapon 
    if (DecreaseCredit(FragmentationGrenadePrice))
    {
        // when player's weapon list is pull
        if (!(K_YGCharacter->PutWepaon(EWeapon::SHOTGUN)))
        {
            // restore credit
            DecreaseCredit(-1 * FragmentationGrenadePrice);
        }
    }
}

void UShopComponent::OnRipleButton1Clicked()
{
    // if credit is enough (DecreaseCredit() is return true)
    // get weapon 
    if (DecreaseCredit(FragmentationGrenadePrice))
    {
        // when player's weapon list is pull
        if (!(K_YGCharacter->PutWepaon(EWeapon::RIFLE)))
        {
            // restore credit
            DecreaseCredit(-1 * FragmentationGrenadePrice);
        }
    }
}

void UShopComponent::OnRipleButton2Clicked()
{
    // if credit is enough (DecreaseCredit() is return true)
    // get weapon 
    if (DecreaseCredit(FragmentationGrenadePrice))
    {
        // when player's weapon list is pull
        if (!(K_YGCharacter->PutWepaon(EWeapon::RIFLE)))
        {
            // restore credit
            DecreaseCredit(-1 * FragmentationGrenadePrice);
        }
    }
   
}

void UShopComponent::OnSniperButtonClicked()
{
    // if credit is enough (DecreaseCredit() is return true)
    // get weapon 
    if (DecreaseCredit(FragmentationGrenadePrice))
    {
        // when player's weapon list is pull
        if (!(K_YGCharacter->PutWepaon(EWeapon::SNIPERRIFLE)))
        {
            // restore credit
            DecreaseCredit(-1 * FragmentationGrenadePrice);
        }
    }

}

void UShopComponent::OnHandGunButtonClicked()
{
    K_YG_SIMPLE(Display);
    // if credit is enough (DecreaseCredit() is return true)
    // get weapon 
    if (DecreaseCredit(FragmentationGrenadePrice))
    {
        // when player's weapon list is pull
        if (!(K_YGCharacter->PutWepaon(EWeapon::HANDGUN)))
        {
            // restore credit
            DecreaseCredit(-1 * FragmentationGrenadePrice);
        }
    } 
}
