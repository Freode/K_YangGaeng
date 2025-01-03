// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "K_YGChatData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EChatMode : uint8
{
	All UMETA(DisplayName = "All"),
	Team UMETA(DisplayName = "Team"),
};

UCLASS()
class K_YANGGAENG_API UK_YGChatData : public UObject
{
    GENERATED_BODY()
public:
    // 기본 생성자
    UK_YGChatData();

    // 메시지의 유형을 저장하는 변수 (모든 플레이어 또는 팀).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat")
    EChatMode ChatMode;

    // 채팅 메시지의 내용을 저장하는 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat")
    FString Content;
};