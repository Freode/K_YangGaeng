// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/ChatSystem/K_YGChatManager.h"
#include "Components/WidgetComponent.h"
#include "Components/EditableTextBox.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Components/ListView.h"
#include "UI/ChatSystem/K_YGOutputLine.h"
#include "UI/ChatSystem/K_YGChatData.h"

// 생성자: 멤버 변수 초기화 및 기본 위젯 클래스 설정
AK_YGChatManager::AK_YGChatManager()
{
    // 채팅 활성 상태 초기화
    bIsChatActive = false;
    // BaseWidget 컴포넌트 생성 및 초기화
    BaseWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BaseWidget"));

    // 기본 위젯 클래스 찾기
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/K_YangGaeng/UI/Chat/WG_Base.WG_Base_C"));
    if (WidgetClassFinder.Succeeded())
    {
        // 위젯 클래스 설정
        BaseWidgetClass = WidgetClassFinder.Class;
    }
    else
    {
        // 클래스 찾기 실패 시 로그 출력
        UE_LOG(LogTemp, Error, TEXT("WidgetClassFinder failed to find class."));
    }
}

// 게임 시작 시 호출되는 함수
void AK_YGChatManager::BeginPlay()
{
    Super::BeginPlay();

    // 첫 번째 플레이어 컨트롤러 가져오기
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    K_YG_CHECK(PlayerController, );

    // EnterKey 액션에 OnEnterPressed 함수 바인딩
    PlayerController->InputComponent->BindAction("EnterKey", IE_Pressed, this, &AK_YGChatManager::OnEnterPressed);

    // 기본 위젯 클래스가 없는 경우 로그 출력 후 함수 종료
    K_YG_CHECK(BaseWidgetClass, );

    // 기본 위젯 생성 및 초기화
    BaseWidget->SetWidgetClass(BaseWidgetClass);
    UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), BaseWidgetClass);
    K_YG_CHECK(WidgetInstance, );

    // 위젯 컴포넌트에 위젯 인스턴스 설정
    BaseWidget->SetWidget(WidgetInstance);
    // 화면에 위젯 추가
    WidgetInstance->AddToViewport();
    // 위젯 초기화 함수 호출
    InitializeWidget(WidgetInstance);
}

// 위젯 초기화 함수
void AK_YGChatManager::InitializeWidget(UUserWidget* WidgetInstance)
{
    K_YG_CHECK(WidgetInstance, );

    // 채팅 위젯 인스턴스 설정
    ChatWidgetInstance = WidgetInstance;

    // 채팅 및 입력 위젯 찾기
    UUserWidget* WG_ChatWidget = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_Chat"));
    K_YG_CHECK(WG_ChatWidget, );

    // 채팅 입력 위젯 찾기
    WG_InputWidget = WG_ChatWidget->WidgetTree->FindWidget<UUserWidget>(FName("WG_Input"));
    K_YG_CHECK(WG_InputWidget, );

    // 채팅 출력 위젯 찾기
    UUserWidget* WG_OutputWidget = WG_ChatWidget->WidgetTree->FindWidget<UUserWidget>(FName("WG_Output"));
    K_YG_CHECK(WG_OutputWidget, );

    // ListView 위젯 찾기
    ListViewChat = WG_OutputWidget->WidgetTree->FindWidget<UListView>(FName("ListViewChat"));
    K_YG_CHECK(ListViewChat, );

    // 입력 위젯 숨기기
    WG_InputWidget->SetVisibility(ESlateVisibility::Hidden);

    // 입력 텍스트 박스 찾기 및 OnTextCommitted 이벤트 바인딩
    InputTextBox = WG_InputWidget->WidgetTree->FindWidget<UEditableTextBox>(FName("EditableTextBox_Content"));
    K_YG_CHECK(InputTextBox && IsValid(InputTextBox), );

    // 텍스트 입력 완료 시 호출될 함수 바인딩
    InputTextBox->OnTextCommitted.AddDynamic(this, &AK_YGChatManager::ProcessChatInput);
}

// 매 프레임 호출되는 함수
void AK_YGChatManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Enter 키가 눌렸을 때 호출되는 함수
void AK_YGChatManager::OnEnterPressed()
{
    K_YG_CHECK(WG_InputWidget, );

    // 채팅창이 숨겨진 경우 활성화
    if (WG_InputWidget->GetVisibility() == ESlateVisibility::Hidden)
    {
        ToggleChat();
    }
    // 채팅창이 보이는 경우 채팅 입력 처리
    else if (WG_InputWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        K_YG_CHECK(InputTextBox, );

        // 입력 텍스트 공백 제거
        FString ChatText = InputTextBox->GetText().ToString().TrimStartAndEnd();
        if (!ChatText.IsEmpty())
        {
            // 채팅 입력 처리 함수 호출
            ProcessChatInput(FText::FromString(ChatText), ETextCommit::OnEnter);
        }
    }
}

// 채팅창의 가시성을 토글하는 함수
void AK_YGChatManager::ToggleChat()
{
    K_YG_CHECK(WG_InputWidget, );

    // 채팅창 가시성 상태에 따라 처리
    if (WG_InputWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        // 채팅창 숨기기
        WG_InputWidget->SetVisibility(ESlateVisibility::Hidden);
        // 입력 텍스트 초기화
        InputTextBox->SetText(FText::GetEmpty());
        // 게임 모드로 입력 모드 설정
        SetInputMode(false);
    }
    else
    {
        // 채팅창 보이기
        WG_InputWidget->SetVisibility(ESlateVisibility::Visible);
        // UI 모드로 입력 모드 설정
        SetInputMode(true);
    }
}

// 입력 모드 설정 함수
void AK_YGChatManager::SetInputMode(bool bChatActive)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    K_YG_CHECK(PlayerController, );

    // 채팅 활성화 여부에 따라 입력 모드 설정 변경
    if (bChatActive)
    {
        // UI 전용 입력 모드 설정
        FInputModeUIOnly InputMode;
        K_YG_CHECK(InputTextBox, );

        // 입력 텍스트 박스에 포커스 설정
        InputMode.SetWidgetToFocus(InputTextBox->TakeWidget());
        PlayerController->SetInputMode(InputMode);
    }
    else
    {
        // 게임 전용 입력 모드 설정
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
    }
}

// 채팅 입력 처리 함수
void AK_YGChatManager::ProcessChatInput(const FText& ChatText, ETextCommit::Type CommitMethod)
{
    // 입력된 채팅 문자열에서 앞뒤 공백 제거
    FString ChatString = ChatText.ToString().TrimStartAndEnd();

    // 공백 제거 후 채팅 문자열이 비어있으면 처리 중단
    K_YG_CHECK(!ChatString.IsEmpty(), );

    // 채팅 메시지 추가 함수 호출
    AddChatMessage(ChatString);

    // FString을 const TCHAR 배열로 변환하여 로그 출력
    const TCHAR* LogMessage = *ChatString;
    UE_LOG(LogTemp, Error, TEXT("%s"), LogMessage);

    // 입력 후 채팅창 닫기
    ToggleChat();
}

// 채팅 메시지 추가 함수
void AK_YGChatManager::AddChatMessage(const FString ChatMessage)
{
    K_YG_CHECK(ListViewChat, );

    // 새로운 채팅 메시지 데이터 생성
    UK_YGChatData* NewChatMessage = NewObject<UK_YGChatData>(this);
    K_YG_CHECK(NewChatMessage, );

    // 채팅 메시지 설정 및 배열에 추가
    NewChatMessage->Content = ChatMessage;
    ChatMessages.Add(NewChatMessage);

    // ListView에 데이터 추가
    ListViewChat->AddItem(NewChatMessage);

    // 로그 추가: 항목이 ListView에 추가되었는지 확인
    UE_LOG(LogTemp, Log, TEXT("Chat message added to ListView: %s"), *ChatMessage);

    // ListView의 스크롤을 가장 아래로 이동
    ListViewChat->ScrollToBottom();
}

// ListView에 채팅 데이터 추가 함수
void AK_YGChatManager::AddMessageToListView(UK_YGChatData* ChatData)
{
    K_YG_CHECK(ListViewChat, );

    // ListView에 채팅 데이터 추가
    ListViewChat->AddItem(ChatData);
}