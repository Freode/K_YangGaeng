// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "LoginComponent.h"
#include "K_YG_LoginTest.generated.h"

UCLASS()
class K_YANGGAENG_API AK_YG_LoginTest : public AK_YGOnlyFightCharacter
{
	GENERATED_BODY()

public:
	AK_YG_LoginTest();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ULoginComponent* LoginComponent;
};
