// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/MainScreen/WG_Credit.h"
#include "Components/Button.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"

void UWG_Credit::NativeConstruct()
{
	Super::NativeConstruct();

	K_YG_CHECK(Button_Back);
	Button_Back->OnClicked.AddDynamic(this, &UWG_Credit::OnBackButtonClicked);
}

void UWG_Credit::OnBackButtonClicked()
{
	K_YG_SIMPLE(Warning);
	//#. PlayerController를 가져와
	if (APlayerController* PC = GetOwningPlayer())
	{
		//#. 내가 만든 controller로 cast하고
		if (AK_YG_LobbyPlayerController* Mypc = Cast<AK_YG_LobbyPlayerController>(PC))
		{
			//#. 함수 실행
			Mypc->Client_SwitchToMain();
		}
	}
}
