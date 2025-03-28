// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "K_YGChatManager.generated.h"

// 전방 선언
class UWidgetComponent;
class UEditableTextBox;
class UUserWidget;
class UWidgetTree;
class APlayerController;
class UListView;
class UK_YGOutputLine;
class UK_YGChatData;

/**
 *
 */
UCLASS()
class K_YANGGAENG_API AK_YGChatManager : public AK_YGOnlyFightCharacter
{
    GENERATED_BODY()

public:
    // 생성자
    AK_YGChatManager();

protected:
    // 게임 시작 시 호출되는 함수
    virtual void BeginPlay() override;

public:
    // 매 프레임마다 호출되는 함수
    virtual void Tick(float DeltaTime) override;

    // Enter 키 입력 시 호출되는 함수
    UFUNCTION()
    void OnEnterPressed();

private:
    // 채팅창 가시성을 전환하는 함수
    void ToggleChat();
    // 입력 모드를 설정하는 함수
    void SetInputMode(bool bChatActive);
    // 위젯 초기화 함수
    void InitializeWidget(UUserWidget* WidgetInstance);

    // 채팅 입력을 처리하는 함수
    UFUNCTION()
    void ProcessChatInput(const FText& ChatText, ETextCommit::Type CommitMethod);

    // 새로운 채팅 메시지를 추가하는 함수
    UFUNCTION()
    void AddChatMessage(const FString ChatMessage);

    // 기본 위젯 클래스 (에디터에서 설정 가능)
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> BaseWidgetClass;

    // 채팅 위젯 인스턴스
    UPROPERTY()
    UUserWidget* ChatWidgetInstance;

    // 입력 위젯
    UPROPERTY()
    UUserWidget* WG_InputWidget;

    // 입력 텍스트 박스
    UPROPERTY()
    UEditableTextBox* InputTextBox;

    // ListView 컴포넌트
    UPROPERTY()
    UListView* ListViewChat;

    // 채팅 활성화 상태 여부
    UPROPERTY()
    bool bIsChatActive;
   
    UPROPERTY()
    UK_YGOutputLine* WG_OutputLine;

    // 기본 위젯 컴포넌트
    UPROPERTY()
    UWidgetComponent* BaseWidget;

    // 채팅 메시지 배열
    UPROPERTY()
    TArray<UK_YGChatData*> ChatMessages; 

    // 추가: ListView에 채팅 메시지 추가 함수
    void AddMessageToListView(UK_YGChatData* ChatData);
    
};