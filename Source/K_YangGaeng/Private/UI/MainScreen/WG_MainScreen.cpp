// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/MainScreen/WG_MainScreen.h"
#include "Components/Button.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "GameFramework/PlayerController.h"

void UWG_MainScreen::NativeConstruct()
{
	Super::NativeConstruct();

	K_YG_CHECK(Button_GameStart);
	Button_GameStart->OnClicked.AddDynamic(this, &UWG_MainScreen::OnGameStartButtonClicked);
	
	K_YG_CHECK(Button_Setting)
	Button_Setting->OnClicked.AddDynamic(this, &UWG_MainScreen::OnSettingButtonClicked);
	
	K_YG_CHECK(Button_Credit)
	Button_Credit->OnClicked.AddDynamic(this, &UWG_MainScreen::OnCreditButtonClicked);
	
	K_YG_CHECK(Button_Exit)
	Button_Exit->OnClicked.AddDynamic(this, &UWG_MainScreen::OnExitButtonClicked);
}

void UWG_MainScreen::OnGameStartButtonClicked()
{
	//#. game start function
	K_YG_SIMPLE(Warning);
	if (APlayerController* PC = GetOwningPlayer())
	{
		K_YG_SIMPLE(Warning);
		//#. 내가 만든 controller로 cast하고
		if (AK_YG_LobbyPlayerController* Mypc = Cast<AK_YG_LobbyPlayerController>(PC))
		{
			K_YG_SIMPLE(Warning);
			//#. 함수 실행
			Mypc->Client_SwitchToStart();
			K_YG_SIMPLE(Warning);
		}
	}
}

void UWG_MainScreen::OnSettingButtonClicked()
{
	//#. game setting function
	K_YG_SIMPLE(Warning);

	if (APlayerController* PC = GetOwningPlayer())
	{
		K_YG_SIMPLE(Warning);
		//#. 내가 만든 controller로 cast하고
		if (AK_YG_LobbyPlayerController* Mypc = Cast<AK_YG_LobbyPlayerController>(PC))
		{
			K_YG_SIMPLE(Warning);
			//#. 함수 실행
			Mypc->Client_SwitchToSetting();
			K_YG_SIMPLE(Warning);
		}
	}
}

void UWG_MainScreen::OnCreditButtonClicked()
{
	//#. credit function
	K_YG_SIMPLE(Warning);

	//#. PlayerController를 가져와
	if (APlayerController* PC = GetOwningPlayer())
	{
		K_YG_SIMPLE(Warning);
		//#. 내가 만든 controller로 cast하고
		if (AK_YG_LobbyPlayerController* Mypc = Cast<AK_YG_LobbyPlayerController>(PC))
		{
			K_YG_SIMPLE(Warning);
			//#. 함수 실행
			Mypc->Client_SwitchToCredit();
			K_YG_SIMPLE(Warning);
		}
	}
}

void UWG_MainScreen::OnExitButtonClicked()
{
	//#. exit function
	K_YG_SIMPLE(Warning);
}

