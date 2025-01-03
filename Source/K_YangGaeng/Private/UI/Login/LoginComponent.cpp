// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/Login/LoginComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ULoginComponent::ULoginComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	//#. Credit widget class 가져오기
	static ConstructorHelpers::FClassFinder<UUserWidget> CreditWidgetClassFinder(TEXT("/Game/Characters/UI/MainScreen/WG_Credit.WG_Credit_C"));
	if (CreditWidgetClassFinder.Succeeded()) {
		CreditWidgetClass = CreditWidgetClassFinder.Class;
	}

	//#. Main widget class 가져오기
	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetClassFinder(TEXT("/Game/Characters/UI/MainScreen/WG_MainScreen.WG_MainScreen_C"));
	if (MainWidgetClassFinder.Succeeded()) {
		MainWidgetClass = MainWidgetClassFinder.Class;
	}

	//#. Login widget class 가져오기
	static ConstructorHelpers::FClassFinder<UUserWidget> LoginWidgetClassFinder(TEXT("/Game/Characters/UI/Login/BP_CreateChar.BP_CreateChar_C"));
	if (LoginWidgetClassFinder.Succeeded()) {
		LoginWidgetClass = LoginWidgetClassFinder.Class;
	}
}

void ULoginComponent::BeginPlay()
{
	Super::BeginPlay();

	if (LoginWidgetClass)
	{
		//#. 플레이어 컨트롤러를 가져오고
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			//#. 캐릭터 마우스 잠금, inputmode 변경
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeUIOnly());

			//#. 컨트롤러가 존재한다면 widget class를 기반으로 하는 Userwidget 만들기
			MainWidget = CreateWidget<UUserWidget>(PlayerController, MainWidgetClass);
			LoginWidget = CreateWidget<UUserWidget>(PlayerController, LoginWidgetClass);
			CreditWidget = CreateWidget<UUserWidget>(PlayerController, CreditWidgetClass);

			if (MainWidget)
			{
				//#. 불러오는데에 성공시 viewport에 띄우고
				MainWidget->AddToViewport();
			}
		}
	}
}
