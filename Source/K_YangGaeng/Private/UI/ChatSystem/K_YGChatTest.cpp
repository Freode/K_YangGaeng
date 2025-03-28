// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/ChatSystem/K_YGChatTest.h"
#include "UI/ChatSystem/K_YGChatComponent.h"
#include "Components/InputComponent.h"

AK_YGChatTest::AK_YGChatTest()
{
    PrimaryActorTick.bCanEverTick = true;

    // UK_YGChatComponent를 생성하고 초기화
    ChatComponent = CreateDefaultSubobject<UK_YGChatComponent>(TEXT("ChatComponent"));
    bReplicates = true;
}

void AK_YGChatTest::BeginPlay()
{
    Super::BeginPlay();
}

void AK_YGChatTest::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
