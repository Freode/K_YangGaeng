// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/GameResult/GameResultComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"

// Sets default values for this component's properties
UGameResultComponent::UGameResultComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    GameResultWidget = nullptr; // 멤버 변수 초기화
}

// 재귀적으로 위젯을 찾는 함수
UWidget* UGameResultComponent::FindWidgetRecursive(UWidget* ParentWidget, const FName& WidgetName)
{
    if (!ParentWidget)
    {
        return nullptr;
    }

    // 현재 위젯이 찾는 위젯인지 확인
    if (ParentWidget->GetFName() == WidgetName)
    {
        return ParentWidget;
    }

    // 자식 위젯들을 재귀적으로 탐색
    UPanelWidget* PanelWidget = Cast<UPanelWidget>(ParentWidget);
    if (PanelWidget)
    {
        for (int32 i = 0; i < PanelWidget->GetChildrenCount(); ++i)
        {
            UWidget* ChildWidget = PanelWidget->GetChildAt(i);
            if (ChildWidget)
            {
                UWidget* FoundWidget = FindWidgetRecursive(ChildWidget, WidgetName);
                if (FoundWidget)
                {
                    return FoundWidget;
                }
            }
        }
    }
    return nullptr;
}


// 위젯 및 하위 컴포넌트를 초기화
void UGameResultComponent::InitializeWidget(UUserWidget* WidgetInstance)
{
    K_YG_CHECK(WidgetInstance, );

    GameResultWidgetInstance = WidgetInstance;


    //같은팀 플레이어 1 위젯 띄우기
    UUserWidget* WG_TeamPlayerScore1 = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_TeamPlalyerScore1"));
    K_YG_CHECK(WG_TeamPlayerScore1, );
    //같은팀 플레이어 2 위젯 띄우기
    UUserWidget* WG_TeamPlayerScore2 = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_TeamPlalyerScore2"));
    K_YG_CHECK(WG_TeamPlayerScore1, );
    //같은팀 플레이어 3 위젯 띄우기
    UUserWidget* WG_TeamPlayerScore3 = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_TeamPlalyerScore3"));
    K_YG_CHECK(WG_TeamPlayerScore1, );
    //같은팀 플레이어 4 위젯 띄우기
    UUserWidget* WG_TeamPlayerScore4 = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_TeamPlalyerScore4"));
    K_YG_CHECK(WG_TeamPlayerScore1, );
    //적팀 플레이어 1 위젯 띄우기
    UUserWidget* WG_EnemyPlayerScore1 = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_EnemyPlayerScore1"));
    K_YG_CHECK(WG_TeamPlayerScore1, );
    //적팀 플레이어 2 위젯 띄우기
    UUserWidget* WG_EnemyPlayerScore2 = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_EnemyPlayerScore2"));
    K_YG_CHECK(WG_TeamPlayerScore1, );
    //적팀 플레이어 3 위젯 띄우기
    UUserWidget* WG_EnemyPlayerScore3 = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_EnemyPlayerScore3"));
    K_YG_CHECK(WG_TeamPlayerScore1, );
    //적팀 플레이어 4 위젯 띄우기
    UUserWidget* WG_EnemyPlayerScore4 = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_EnemyPlayerScore4"));
    K_YG_CHECK(WG_TeamPlayerScore1, );

    //하위 위젯 속성 Hidden -> Visible로 바꾸기
    //WG_TeamPlayerScore1->SetVisibility(ESlateVisibility::Visible);
    //WG_TeamPlayerScore2->SetVisibility(ESlateVisibility::Visible);
    //WG_TeamPlayerScore3->SetVisibility(ESlateVisibility::Visible);
    //WG_TeamPlayerScore4->SetVisibility(ESlateVisibility::Visible);

    //WG_EnemyPlayerScore1->SetVisibility(ESlateVisibility::Visible);
    //WG_EnemyPlayerScore2->SetVisibility(ESlateVisibility::Visible);
    //WG_EnemyPlayerScore3->SetVisibility(ESlateVisibility::Visible);
    //WG_EnemyPlayerScore4->SetVisibility(ESlateVisibility::Visible);

    
}

// BeginPlay에서 위젯을 초기화하고 업데이트
void UGameResultComponent::BeginPlay()
{
    Super::BeginPlay();

    // 패키지 경로로부터 직접 클래스 로드
    FSoftObjectPath WidgetClassReference(TEXT("/Game/K_YangGaeng/UI/GameResult/WG_GameResult.WG_GameResult_C"));
    UClass* WidgetClass = Cast<UClass>(WidgetClassReference.TryLoad());

    if (WidgetClass)
    {
        this->GameResultWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
        if (this->GameResultWidgetInstance)
        {
            this->GameResultWidgetInstance->AddToViewport();
            InitializeWidget(this->GameResultWidgetInstance);
        }
    }
}

// Called every frame
void UGameResultComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    
}


void UGameResultComponent::ServerSendNumberOfPlayer_Implementation(int32 PlayerNumber)
{

}

bool UGameResultComponent::ServerSendNumberOfPlayer_Validate(int32 PlayerNumber)
{
    return true;
}

void UGameResultComponent::MulticastReceiveNumberOfPlayer_Implementation(int32 PlayerNumber)
{

}
