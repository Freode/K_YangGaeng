// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/Login/K_YG_LoginTest.h"

AK_YG_LoginTest::AK_YG_LoginTest()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and attach the LoginComponent
	LoginComponent = CreateDefaultSubobject<ULoginComponent>(TEXT("LoginComponent"));
}

void AK_YG_LoginTest::BeginPlay()
{
	Super::BeginPlay();
}
