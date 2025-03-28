// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "K_YGOutputLine.generated.h"

// 전방 선언
class UTextBlock;
class UK_YGChatData;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UK_YGOutputLine : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

public:
    // 채팅 메시지를 포함하는 컨테이너
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VerticalBoxAutoContent;

    // 채팅 메시지의 텍스트 내용을 표시하는 위젯
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TextBlockContent;

    // 채팅 데이터를 설정하고 위젯에 반영
    void SetChatData(UK_YGChatData* InChatData);

    // 현재 설정된 채팅 메시지 데이터를 저장
    UK_YGChatData* ChatData;

    // IUserObjectListEntry 인터페이스 함수

    // ListItemObject가 설정될 때 호출됩
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
    // NativeConstruct 함수는 위젯이 생성될 때 호출
    virtual void NativeConstruct() override;
};
