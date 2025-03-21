// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/Setting/WG_Setting_Button.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWG_Setting_Button::NativeConstruct()
{
	Super::NativeConstruct();

	//#. initialize
	bIsButtonClicked = true;

	//#. 함수 바인딩
	if (Button_Select) {
		Button_Select->OnClicked.AddDynamic(this, &UWG_Setting_Button::OnButtonClicked);
	}
}

//#. button click 시 외관 함수 실행
//#. Widget의 형태 설정을 여기서 설정
//#. 기능 구현은 Setting에서 구현
//#. 또한 Setting에서 불러올 수 있도록 내용을 변경하는 함수 1개 더 생성
void UWG_Setting_Button::OnButtonClicked()
{
	K_YG_SIMPLE(Warning);
	//#. 현재 Enable 상태
	if (bIsButtonClicked)
	{
		//#. 처음 버튼을 누를 경우 Disable 상태가 됨
		bIsButtonClicked = false;
	}
	else
	{
		//#. 다시 누를 경우 원래 Enable상태가 됨
		bIsButtonClicked = true;
	}
	OnButtonPressed(bIsButtonClicked);

}

void UWG_Setting_Button::OnButtonPressed(bool Value)
{
	if (Value)
	{
		TextBlock_Enable->SetText(FText::FromString("Enable"));
	}
	else
	{
		TextBlock_Enable->SetText(FText::FromString("Disable"));
	}
}

UButton* UWG_Setting_Button::GetButton()
{
	return Button_Select;
}

void UWG_Setting_Button::SetTitle(FString Title)
{
	TextBlock_Name->SetText(FText::FromString(Title));
}


