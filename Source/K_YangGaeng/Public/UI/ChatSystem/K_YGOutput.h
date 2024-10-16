// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "K_YGOutput.generated.h"

class UListView;
class UK_YGChatData;

/**
 *
 */
UCLASS()
class K_YANGGAENG_API UK_YGOutput : public UUserWidget
{
    GENERATED_BODY()

public:
    // 채팅 메시지를 리스트 형태로 표시하는 위젯
    UPROPERTY(meta = (BindWidget))
    UListView* ListViewChat;

protected:
    // 위젯이 생성될 때 호출
    virtual void NativeConstruct() override;
};