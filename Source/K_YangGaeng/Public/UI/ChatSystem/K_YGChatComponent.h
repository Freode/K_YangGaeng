// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "K_YGChatData.h"
#include "K_YGChatComponent.generated.h"

// 전방 선언
class UWidgetComponent;
class UEditableTextBox;
class UUserWidget;
class UWidgetTree;
class APlayerController;
class UListView;
class UK_YGOutputLine;
class UK_YGChatData;
class UTextBlock;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UK_YGChatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // 기본 생성자
    UK_YGChatComponent();

protected:
    // 게임 시작 시 호출되는 메서드
    virtual void BeginPlay() override;

    // 포커스 상태를 추적하기 위한 변수
    bool bWasTextBoxFocused;

public:
    // 매 프레임마다 호출되는 메서드
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Enter 키가 눌렸을 때 호출되는 메서드
    UFUNCTION()
    void OnEnterPressed();

    virtual void SetupInputComponent(UInputComponent* InputComponent);

private:
    // 채팅 위젯의 가시성을 토글하는 메서드
    void ToggleChat();

    // 입력 모드를 설정하는 메서드
    // 채팅이 활성화된 경우 UI 전용 모드로 설정하고 그렇지 않으면 게임 모드로 설정
    void SetInputMode(bool bChatActive);

    // 위젯 및 하위 컴포넌트를 초기화하는 메서드
    void InitializeWidget(UUserWidget* WidgetInstance);

    // 채팅 입력을 처리하는 메서드
    // 채팅 입력 텍스트와 커밋을 처리
    UFUNCTION()
    void ProcessChatInput(const FText& ChatText, ETextCommit::Type CommitMethod);

    // 채팅 메시지를 추가하는 메서드
    UFUNCTION()
    void AddChatMessage(const FString ChatMessage);

    // UI에서 사용할 위젯 클래스 지정
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> BaseWidgetClass;

    // 채팅 위젯의 인스턴스를 저장하는 변수
    UPROPERTY()
    UUserWidget* ChatWidgetInstance;

    // 입력 위젯을 저장하는 변수
    UPROPERTY()
    UUserWidget* WG_InputWidget;

    // 입력 텍스트 박스를 저장하는 변수
    UPROPERTY()
    UEditableTextBox* InputTextBox;

    // 채팅 메시지를 표시할 ListView를 저장하는 변수
    UPROPERTY()
    UListView* ListViewChat;

    // 채팅이 활성화된 상태를 저장하는 변수
    UPROPERTY()
    bool bIsChatActive;

    // 채팅 출력 라인을 저장하는 변수
    UPROPERTY()
    UK_YGOutputLine* WG_OutputLine;

    // 현재 ChatMode를 표시하는 텍스트 블록
    UPROPERTY()
    UTextBlock* ModeTextBlock;

    // BaseWidget 컴포넌트를 저장하는 변수
    UPROPERTY()
    UWidgetComponent* BaseWidget;

    // 채팅 메시지 객체들을 저장하는 배열
    UPROPERTY()
    TArray<UK_YGChatData*> ChatMessages;

    // 현재 채팅 모드를 저장하는 변수 
    UPROPERTY()
    EChatMode CurrentChatMode;

    // 채팅 데이터를 ListView에 추가하는 메서드
    void AddMessageToListView(UK_YGChatData* ChatData);

    // 채팅 데이터를 목록에 추가하고 목록을 새로 고치는 메서드
    void AddChatLine(UK_YGChatData* ChatData);

    // Tab 키 입력을 처리하여 ChatMode를 변경
    //void OnTabPressed();

    // ChatMode에 따라 텍스트 블록을 업데이트
    void UpdateChatModeText();

    // 키 입력을 처리할 함수
    void HandleTabPressed();


// =================================================================
// =============================Server==============================
// =================================================================

// 서버에서 호출되어 메시지를 브로드캐스트하는 메서드
public:
    // 서버로 채팅 메시지를 전송
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerSendChatMessage(const FString& ChatMessage);

    // 클라이언트에게 채팅 메시지를 브로드캐스트
    UFUNCTION(NetMulticast, Reliable)
    void MulticastReceiveMessage(const FString& ChatMessage);


protected:

    // 서버에서 호출하기 위한 함수
    void ServerSendChatMessage_Implementation(const FString& ChatMessage);

    // 서버 호출의 유효성 검사 함수
    bool ServerSendChatMessage_Validate(const FString& ChatMessage);

    // 브로드캐스트 메시지 처리 함수
    void MulticastReceiveMessage_Implementation(const FString& ChatMessage);
};
