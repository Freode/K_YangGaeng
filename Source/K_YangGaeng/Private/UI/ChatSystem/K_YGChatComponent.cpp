// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/ChatSystem/K_YGChatComponent.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "UI/Login/WG_Login.h"
#include "Components/WidgetComponent.h"
#include "Components/EditableTextBox.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"
#include "Components/ListView.h"
#include "UI/ChatSystem/K_YGOutputLine.h"
#include "UI/ChatSystem/K_YGChatData.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Framework/Application/SlateApplication.h"
#include "SlateOptMacros.h"

// 이 컴포넌트의 기본값 설정
UK_YGChatComponent::UK_YGChatComponent()
{
    // 이 컴포넌트가 매 프레임마다 업데이트되도록 설정
    PrimaryComponentTick.bCanEverTick = true;

    // 컴포넌트가 복제되도록 설정
    SetIsReplicatedByDefault(true);

    // 채팅 활성화 상태를 초기화
    bIsChatActive = false;

    // 포커스 상태 초기화
    bWasTextBoxFocused = false;

    // 초기 채팅 모드를 설정
    CurrentChatMode = EChatMode::Team;

    // 위젯 컴포넌트 생성
    BaseWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BaseWidget"));

    // 위젯 클래스를 찾는 과정
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/K_YangGaeng/UI/Chat/WG_Base.WG_Base_C"));
    if (WidgetClassFinder.Succeeded())
    {
        // 위젯 클래스를 성공적으로 찾은 경우 클래스 변수에 저장
        BaseWidgetClass = WidgetClassFinder.Class;
    }
}

// 게임이 시작될 때 호출
void UK_YGChatComponent::BeginPlay()
{
    // 부모 클래스의 BeginPlay 호출
    Super::BeginPlay();

    // 플레이어 컨트롤러 가져오기
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    // PlayerController가 null이면 함수 종료
    K_YG_CHECK(PlayerController, );

    // "EnterKey" 입력 액션을 처리하기 위한 바인딩 설정
    PlayerController->InputComponent->BindAction("EnterKey", IE_Pressed, this, &UK_YGChatComponent::OnEnterPressed);

    // "Tab" 키 입력 액션을 처리하기 위한 바인딩 설정
    //PlayerController->InputComponent->BindAction("TabKey", IE_Pressed, this, &UK_YGChatComponent::OnTabPressed);

    // BaseWidgetClass가 null이면 함수 종료
    K_YG_CHECK(BaseWidgetClass, );

    // BaseWidget에 위젯 클래스를 설정
    BaseWidget->SetWidgetClass(BaseWidgetClass);
    UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), BaseWidgetClass);
    // WidgetInstance가 null이면 함수 종료
    K_YG_CHECK(WidgetInstance, );

    // 위젯 인스턴스를 BaseWidget에 설정하고 뷰포트에 추가
    BaseWidget->SetWidget(WidgetInstance);
    WidgetInstance->AddToViewport();
    // 위젯 및 하위 컴포넌트 초기화
    InitializeWidget(WidgetInstance);

    AActor* OwnerActor = GetOwner();
    if (OwnerActor)
    {
        ENetRole Role = OwnerActor->GetLocalRole();
        UE_LOG(LogTemp, Log, TEXT("BeginPlay - Owner Role: %d"), (int32)Role);

        if (Role == ROLE_Authority)
        {
            UE_LOG(LogTemp, Log, TEXT("Server is Authority"));
        }
    }

    // InputComponent 초기화 및 키 바인딩
    if (AActor* Owner = GetOwner())
    {
        UInputComponent* InputComponent = Owner->FindComponentByClass<UInputComponent>();
        if (InputComponent)
        {
            InputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &UK_YGChatComponent::HandleTabPressed);
        }
    }
}

// 매 프레임마다 호출
void UK_YGChatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    // 부모 클래스의 TickComponent 호출
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsChatActive && InputTextBox)
    {
        const bool bIsCurrentlyFocused = InputTextBox->HasKeyboardFocus();

        // 포커스를 잃었다가 다시 얻지 못한 경우
        if (!bIsCurrentlyFocused && bWasTextBoxFocused)
        {
            // 다시 포커스 설정
            // 마우스 클릭으로도 다시 포커스 설정
            InputTextBox->SetKeyboardFocus();  
            InputTextBox->SetFocus();          
        }

        // 현재 포커스 상태를 저장
        bWasTextBoxFocused = bIsCurrentlyFocused;
    }
}

// 위젯 및 하위 컴포넌트를 초기화
void UK_YGChatComponent::InitializeWidget(UUserWidget* WidgetInstance)
{
    // WidgetInstance가 null이면 함수 종료
    K_YG_CHECK(WidgetInstance, );
    // ChatWidgetInstance 설정
    ChatWidgetInstance = WidgetInstance;

    // WG_Chat 위젯을 찾기
    UUserWidget* WG_ChatWidget = WidgetInstance->WidgetTree->FindWidget<UUserWidget>(FName("WG_Chat"));
    K_YG_CHECK(WG_ChatWidget, );

    // WG_Input 위젯을 찾기
    WG_InputWidget = WG_ChatWidget->WidgetTree->FindWidget<UUserWidget>(FName("WG_Input"));
    K_YG_CHECK(WG_InputWidget, );

    // WG_Output 위젯을 찾기
    UUserWidget* WG_OutputWidget = WG_ChatWidget->WidgetTree->FindWidget<UUserWidget>(FName("WG_Output"));
    K_YG_CHECK(WG_OutputWidget, );

    // ListViewChat 위젯을 찾기
    ListViewChat = WG_OutputWidget->WidgetTree->FindWidget<UListView>(FName("ListViewChat"));
    K_YG_CHECK(ListViewChat, );

    // 채팅 입력 위젯을 숨김
    WG_InputWidget->SetVisibility(ESlateVisibility::Hidden);

    // Mode 텍스트 블록 찾기
    ModeTextBlock = WG_InputWidget->WidgetTree->FindWidget<UTextBlock>(FName("Mode")); 
    K_YG_CHECK(ModeTextBlock, );

    // 입력 텍스트 박스를 찾기
    InputTextBox = WG_InputWidget->WidgetTree->FindWidget<UEditableTextBox>(FName("EditableTextBox_Content"));
    K_YG_CHECK(InputTextBox && IsValid(InputTextBox), );  

    // 텍스트가 커밋될 때 호출될 메서드를 추가
    InputTextBox->OnTextCommitted.AddDynamic(this, &UK_YGChatComponent::ProcessChatInput);

    SetIsReplicatedByDefault(true);
    SetNetAddressable();

    // 초기 채팅 모드를 Mode 텍스트 블록에 표시
    UpdateChatModeText();

}


// Enter 키 입력을 처리하여 채팅 입력을 보이게 하거나 숨김
void UK_YGChatComponent::OnEnterPressed()
{
    // WG_InputWidget이 null이면 함수 종료
    K_YG_CHECK(WG_InputWidget, );

    // 채팅 입력 위젯이 숨겨져 있는 경우 채팅을 토글
    if (WG_InputWidget->GetVisibility() == ESlateVisibility::Hidden)
    {
        ToggleChat();
    }
    // 채팅 입력 위젯이 보이는 경우 입력 텍스트를 처리
    else if (WG_InputWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        // InputTextBox가 null이면 함수 종료
        K_YG_CHECK(InputTextBox, );

        // 텍스트를 가져와서 앞뒤 공백을 제거
        FString ChatText = InputTextBox->GetText().ToString().TrimStartAndEnd();

        if (ChatText.IsEmpty())
        {
            // 입력 모드를 게임 모드로 설정
            SetInputMode(false);
            // 채팅 숨김
            ToggleChat();
        }
        else
        {
            // 채팅 입력 처리
            ProcessChatInput(FText::FromString(ChatText), ETextCommit::OnEnter);
        }
    }
}

// 탭 키가 눌렸을 때 호출되는 함수
void UK_YGChatComponent::HandleTabPressed()
{
    // 채팅 모드를 전환
    CurrentChatMode = (CurrentChatMode == EChatMode::Team) ? EChatMode::All : EChatMode::Team;

    // 전환된 채팅 모드를 화면에 업데이트
    UpdateChatModeText();

    // 만약 채팅 입력 위젯이 보이고 있다면, 텍스트 박스의 포커스를 유지
    if (WG_InputWidget && WG_InputWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        if (InputTextBox && InputTextBox->HasKeyboardFocus())
        {
            InputTextBox->SetKeyboardFocus();
        }
    }
}

// SetupInputComponent에서 탭 키 입력을 처리하도록 설정
void UK_YGChatComponent::SetupInputComponent(UInputComponent* InputComponent)
{
    if (InputComponent)
    {
        // Tab 키 입력을 HandleTabPressed 함수에 바인딩
        InputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &UK_YGChatComponent::HandleTabPressed);
    }
}




// ChatMode에 따라 텍스트 블록을 업데이트
void UK_YGChatComponent::UpdateChatModeText()
{
    K_YG_CHECK(ModeTextBlock, );

    // 현재 채팅 모드를 반영한 텍스트를 설정
    FString ModeText = (CurrentChatMode == EChatMode::Team) ? TEXT("Team") : TEXT("All");
    ModeTextBlock->SetText(FText::FromString(ModeText));

    // 채팅 입력 위젯이 보이고 있는 경우 포커스를 유지
    if (InputTextBox && WG_InputWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        InputTextBox->SetKeyboardFocus();
    }
}

// 채팅 입력 위젯의 가시성을 토글하고 입력 모드 업데이트
void UK_YGChatComponent::ToggleChat()
{
    // WG_InputWidget이 null이면 함수 종료
    K_YG_CHECK(WG_InputWidget, );

    // 채팅 입력 위젯이 숨겨져 있으면 보이도록 설정
    if (WG_InputWidget->GetVisibility() == ESlateVisibility::Hidden)
    {
        WG_InputWidget->SetVisibility(ESlateVisibility::Visible);
        // 채팅이 활성화되었음을 표시
        bIsChatActive = true;

        // 플레이어 컨트롤러를 가져옴
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            // UI 전용 입력 모드를 설정
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(InputTextBox->TakeWidget());  
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            // InputMode를 설정하고 마우스 커서를 표시
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;

            // EditableTextBox에 키보드 포커스도 
            InputTextBox->SetKeyboardFocus();
        }
    }
    // 채팅 입력 위젯이 보이면 숨김
    else
    {
        WG_InputWidget->SetVisibility(ESlateVisibility::Hidden);
        // 채팅이 비활성화되었음을 표시
        bIsChatActive = false;
        // 입력 모드를 게임 모드로 설정
        SetInputMode(false);
    }
}

// 입력 모드를 설정
// 채팅이 활성화된 경우 UI 전용 모드로 설정하고 그렇지 않으면 게임 모드로 설정
void UK_YGChatComponent::SetInputMode(bool bChatActive)
{
    // 플레이어 컨트롤러를 가져옴
    // PlayerController가 null이면 함수 종료
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    K_YG_CHECK(PlayerController, );

    if (bChatActive)
    {
        // UI 전용 입력 모드를 
        // InputTextBox가 null이면 함수 종료
        FInputModeUIOnly InputMode;
        K_YG_CHECK(InputTextBox, );


        InputMode.SetWidgetToFocus(InputTextBox->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
        PlayerController->SetInputMode(InputMode);
        // 마우스 커서 표시
        PlayerController->bShowMouseCursor = true;
    }
    else
    {
        // 게임 전용 입력 모드를 설정
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        // 마우스 커서 숨김
        PlayerController->bShowMouseCursor = false;
    }
}

// 채팅 입력을 처리
void UK_YGChatComponent::ProcessChatInput(const FText& Text, ETextCommit::Type CommitMethod)
{
    UE_LOG(LogTemp, Log, TEXT("ProcessChatInput called"));
    if (CommitMethod == ETextCommit::OnEnter)
    {
        // 입력된 텍스트를 가져와 앞뒤 공백을 제거한 후 문자열로 변환
        FString InputText = Text.ToString().TrimStartAndEnd();
        UE_LOG(LogTemp, Log, TEXT("Processed Input Text: %s"), *InputText);

        if (!InputText.IsEmpty())
        {
            // 입력 텍스트 박스가 유효한 경우 텍스트를 비움
            if (InputTextBox)
            {
                InputTextBox->SetText(FText::GetEmpty());
            }

            // 소유자 액터 가져오기
            AActor* OwnerActor = GetOwner();
            if (OwnerActor)
            {
                // 소유자 액터의 네트워크 역할 가져오기
                ENetRole Role = OwnerActor->GetLocalRole();
                UE_LOG(LogTemp, Log, TEXT("Owner Role: %d"), (int32)Role);

                // 서버인 경우 메시지 브로드캐스트
                if (Role == ROLE_Authority)
                {
                    MulticastReceiveMessage(InputText);
                }
                // 클라이언트인 경우 서버로 메시지 전송 요청
                else if (Role == ROLE_AutonomousProxy)
                {
                    UE_LOG(LogTemp, Log, TEXT("Calling ServerSendChatMessage from client"));
                    ServerSendChatMessage(InputText);
                    UE_LOG(LogTemp, Log, TEXT("Client sending message to server: %s"), *InputText);
                }
                // 예기치 않은 역할인 경우 경고 로그 기록
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Unexpected Role: %d"), (int32)Role);
                }
            }

            // 입력 모드를 게임 모드로 설정
            SetInputMode(false);
            // 채팅 숨김
            ToggleChat();
        }
        else
        {
            // 텍스트가 비어 있는 경우 채팅을 숨김
            SetInputMode(false);
            ToggleChat();
        }
    }
}


// 채팅 메시지를 ListView에 추가
void UK_YGChatComponent::AddChatMessage(const FString ChatMessage)
{
    // ListViewChat이 null이면 함수 종료
    K_YG_CHECK(ListViewChat, );

    // 새로운 채팅 메시지 객체를 생성
    UK_YGChatData* NewChatMessage = NewObject<UK_YGChatData>(this);
    // NewChatMessage가 null이면 함수 종료
    K_YG_CHECK(NewChatMessage, );

    // 채팅 데이터의 내용 설정
    NewChatMessage->Content = ChatMessage;
    // 새로운 채팅 메시지를 목록에 추가
    ChatMessages.Add(NewChatMessage);

    // 새 메시지를 ListView에 추가
    AddMessageToListView(NewChatMessage);
}

// 채팅 데이터를 ListView에 추가
void UK_YGChatComponent::AddMessageToListView(UK_YGChatData* ChatData)
{
    // ChatData 또는 ListViewChat이 null이면 함수 종료
    K_YG_CHECK(ChatData && ListViewChat, );
    // ListView에 채팅 데이터 추가
    ListViewChat->AddItem(ChatData);
    // 추가된 메시지를 즉시 반영
    ListViewChat->RequestRefresh();
    // 최신 메시지가 보이도록 스크롤
    ListViewChat->ScrollToBottom();
}

// 채팅 데이터를 목록에 추가하고 목록 새로 고침
void UK_YGChatComponent::AddChatLine(UK_YGChatData* ChatData)
{
    // ListViewChat 또는 ChatData가 null이면 함수 종료
    K_YG_CHECK(ListViewChat && ChatData, );

    // ListView에 채팅 데이터 추가
    ListViewChat->AddItem(ChatData);
    // ListView를 새로 고침
    ListViewChat->RequestRefresh();
    // ListView의 스크롤을 가장 아래로 이동
    ListViewChat->ScrollToBottom();
}



// =================================================================
// =============================Server==============================
// =================================================================

// 서버가 클라이언트로부터 채팅 메시지를 받아 처리 
void UK_YGChatComponent::ServerSendChatMessage_Implementation(const FString& ChatMessage)
{
    AActor* OwnerActor = GetOwner();
    if (OwnerActor)
    {
        // PlayerController를 가져옴
        AK_YG_LobbyPlayerController* PlayerController = Cast<AK_YG_LobbyPlayerController>(OwnerActor);
        if (PlayerController && PlayerController->PlayerState)
        {
            // PlayerState에서 닉네임 가져오기
            FString PlayerName = PlayerController->PlayerState->GetPlayerName();
            FString FullMessage = FString::Printf(TEXT("[%s]: %s"), *PlayerName, *ChatMessage);

            // 서버가 권한을 가지고 있다면 메시지를 브로드캐스트
            if (OwnerActor->GetLocalRole() == ROLE_Authority)
            {
                MulticastReceiveMessage(FullMessage);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("ServerSendChatMessage_Implementation called without authority"));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ServerSendChatMessage_Implementation called with no owner"));
    }
}


// 서버가 클라이언트로부터 받은 채팅 메시지의 유효성을 검증
bool UK_YGChatComponent::ServerSendChatMessage_Validate(const FString& ChatMessage)
{
    UE_LOG(LogTemp, Log, TEXT("ServerSendChatMessage_Validate called with message: %s"), *ChatMessage);
    // 기본적으로 모든 메시지를 유효한 것으로 간주
    return true;  
}

// 클라이언트에게 채팅 메시지를 브로드캐스트
void UK_YGChatComponent::MulticastReceiveMessage_Implementation(const FString& ChatMessage)
{
    // 채팅 메시지를 추가
    AddChatMessage(ChatMessage);
    UE_LOG(LogTemp, Log, TEXT("Client received message: %s"), *ChatMessage);
}