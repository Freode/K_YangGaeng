// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "K_YGChatTest.generated.h"

class UK_YGChatComponent;
/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGChatTest : public AK_YGOnlyFightCharacter
{
	GENERATED_BODY()

public:
    AK_YGChatTest();


protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chat", meta = (AllowPrivateAccess = "true"))
    UK_YGChatComponent* ChatComponent;
};
