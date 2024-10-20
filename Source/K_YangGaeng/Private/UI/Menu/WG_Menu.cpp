// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/Menu/WG_Menu.h"
#include "Components/Button.h"
#include "Components/InputComponent.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"

void UWG_Menu::NativeConstruct()
{
	Super::NativeConstruct();

	//#. 키바인딩을 위해 포커스를 true로 설정
	bIsFocusable = true;

	K_YG_CHECK(Button_Play);
	Button_Play->OnClicked.AddDynamic(this, &UWG_Menu::OnPlayButtonClicked);

	K_YG_CHECK(Button_Graphic);
	Button_Graphic->OnClicked.AddDynamic(this, &UWG_Menu::OnGraphicButtonClicked);

	K_YG_CHECK(Button_Audio);
	Button_Audio->OnClicked.AddDynamic(this, &UWG_Menu::OnAudioButtonClicked);

	K_YG_CHECK(Button_Quit);
	Button_Quit->OnClicked.AddDynamic(this, &UWG_Menu::OnQuitButtonClicked);
}

void UWG_Menu::OnPlayButtonClicked()
{
	K_YG_SIMPLE(Warning);
}

void UWG_Menu::OnGraphicButtonClicked()
{
	K_YG_SIMPLE(Warning);

}

void UWG_Menu::OnAudioButtonClicked()
{
	K_YG_SIMPLE(Warning);

}

void UWG_Menu::OnQuitButtonClicked()
{
	K_YG_SIMPLE(Warning);
	RemoveFromParent();

	Mypc = Cast<AK_YG_LobbyPlayerController>(GetOwningPlayer());
	Mypc->SetInputMode(FInputModeGameOnly());
	Mypc->bShowMouseCursor = false;
}

FReply UWG_Menu::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InputKeyEvent)
{
	K_YG_SIMPLE(Warning);
	if (InputKeyEvent.GetKey() == EKeys::F)
	{
		K_YG_SIMPLE(Warning);
		RemoveFromParent();

		Mypc = Cast<AK_YG_LobbyPlayerController>(GetOwningPlayer());
		Mypc->SetInputMode(FInputModeGameOnly());
		Mypc->bShowMouseCursor = false;

		return FReply::Handled();

	}
	return Super::NativeOnKeyDown(MyGeometry, InputKeyEvent);
}