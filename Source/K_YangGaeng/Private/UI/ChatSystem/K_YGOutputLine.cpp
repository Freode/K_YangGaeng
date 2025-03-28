// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/ChatSystem/K_YGOutputLine.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/ChatSystem/K_YGChatData.h"

// ListItemObject가 설정될 때 호출되는 함수
void UK_YGOutputLine::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    // 로그 출력: 함수 호출 확인
    UE_LOG(LogTemp, Log, TEXT("NativeOnListItemObjectSet called"));

    // 부모 클래스의 NativeOnListItemObjectSet 호출
    IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

    // ListItemObject를 UK_YGChatData 타입으로 캐스팅
    UK_YGChatData* ChatDataItem = Cast<UK_YGChatData>(ListItemObject);
    if (ChatDataItem)
    {
        // 캐스팅 성공 시 채팅 데이터 설정 함수 호출
        SetChatData(ChatDataItem);
    }
    else
    {
        // 캐스팅 실패 시 경고 로그 출력
        UE_LOG(LogTemp, Warning, TEXT("ListItemObject is not of type UK_YGChatData"));
    }
}

// 채팅 데이터를 설정하는 함수
void UK_YGOutputLine::SetChatData(UK_YGChatData* InChatData)
{
    // 멤버 변수에 채팅 데이터 설정
    ChatData = InChatData;

    // TextBlockContent와 ChatData가 유효한지 확인
    if (TextBlockContent && ChatData)
    {
        // TextBlockContent에 채팅 내용 설정
        TextBlockContent->SetText(FText::FromString(ChatData->Content));

        // 로그 출력: 채팅 내용 설정 확인
        UE_LOG(LogTemp, Log, TEXT("Chat content set: %s"), *ChatData->Content);
    }
    else
    {
        // TextBlockContent 또는 ChatData가 null인 경우 경고 로그 출력
        UE_LOG(LogTemp, Warning, TEXT("TextBlockContent or ChatData is null"));
    }
}

// 위젯이 생성될 때 호출되는 함수
void UK_YGOutputLine::NativeConstruct()
{
    // 부모 클래스의 NativeConstruct 호출
    Super::NativeConstruct();

    // VerticalBoxAutoContent 위젯 바인딩
    if (!VerticalBoxAutoContent)
    {
        // 위젯 트리에서 VerticalBoxAutoContent 위젯 찾기
        VerticalBoxAutoContent = Cast<UVerticalBox>(GetWidgetFromName(TEXT("VerticalBoxAutoContent")));
        if (VerticalBoxAutoContent)
        {
            // 로그 출력: 위젯 찾기 성공
            UE_LOG(LogTemp, Log, TEXT("VerticalBoxAutoContent found"));
        }
        else
        {
            // 로그 출력: 위젯 찾기 실패
            UE_LOG(LogTemp, Error, TEXT("VerticalBoxAutoContent not found"));
        }
    }

    // TextBlockContent 위젯 바인딩
    if (!TextBlockContent)
    {
        // 위젯 트리에서 TextBlockContent 위젯 찾기
        TextBlockContent = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlockContent")));
        if (TextBlockContent)
        {
            // 로그 출력: 위젯 찾기 성공
            UE_LOG(LogTemp, Log, TEXT("TextBlockContent found"));
        }
        else
        {
            // 로그 출력: 위젯 찾기 실패
            UE_LOG(LogTemp, Error, TEXT("TextBlockContent not found"));
        }
    }
}
