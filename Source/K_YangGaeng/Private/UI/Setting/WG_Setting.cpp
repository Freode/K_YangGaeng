// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/Setting/WG_Setting.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "UI/Setting/WG_Setting_Slide.h"
#include "UI/Setting/WG_Setting_Button.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "Kismet/GameplayStatics.h"



void UWG_Setting::NativeConstruct()
{
	Super::NativeConstruct();

	//#. initialize
	Mypc = Cast<AK_YG_LobbyPlayerController>(GetOwningPlayer());

	if (Mypc)
	{
		CurrentMasterVolume = Mypc->GetPlayerState<AK_YG_LobbyPlayerState>()->GetMasterVolume();
		CurrentEffectVolume = Mypc->GetPlayerState<AK_YG_LobbyPlayerState>()->GetEffectVolume();
		CurrentIsFullScreen = Mypc->GetPlayerState<AK_YG_LobbyPlayerState>()->GetIsFullScreen();
		UE_LOG(LogTemp, Log, TEXT("%c"), CurrentIsFullScreen ? 'T' : 'F');
	}

	//#. index 0 : MasterVolume Control
	if (MasterVolumeWidget) {
		K_YG_SIMPLE(Warning);
		USlider* MasterVolumeSlider = MasterVolumeWidget->GetSlider();
		if (MasterVolumeSlider) {
			MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UWG_Setting::OnMasterVolumeChanged);
			MasterVolumeWidget->SetTitle("Master Volume");

			//#. 저장되어있는 상태를 불러오기
			MasterVolumeWidget->OnSlideChanged(CurrentMasterVolume);
		}
	}

	//#. index 1 : isFullScreen Control
	if (FullScreenWidget) {
		K_YG_SIMPLE(Warning);
		UButton* FullScreenButton = FullScreenWidget->GetButton();
		if (FullScreenButton) {
			FullScreenButton->OnClicked.AddDynamic(this, &UWG_Setting::OnFullScreenChanged);
			FullScreenWidget->SetTitle("FullScreen");

			//#. 저장되어있는 상태를 불러오기
			//#. 버튼을 누르는것과 동일 효과...불러오려면 어케해야할까
			FullScreenWidget->OnButtonPressed(CurrentIsFullScreen);
		}
	}

	//#. index 3 : EffectVolume Control
	if (EffectVolumeWidget) {
		K_YG_SIMPLE(Warning);
		USlider* EffectVolumeSlider = EffectVolumeWidget->GetSlider();
		if (EffectVolumeSlider) {
			EffectVolumeSlider->OnValueChanged.AddDynamic(this, &UWG_Setting::OnEffectVolumeChanged);
			EffectVolumeWidget->SetTitle("Effect Volume");

			//#. 저장되어있는 상태를 불러오기
			EffectVolumeWidget->OnSlideChanged(CurrentEffectVolume);
		}
	}
	
	//#. Back button clicked
	K_YG_CHECK(Button_Back);
	Button_Back->OnClicked.AddDynamic(this, &UWG_Setting::OnBackButtonClicked);
}
/////////////////////////////////////////////////////////////////

void UWG_Setting::OnBackButtonClicked()
{
	K_YG_SIMPLE(Warning);
	Mypc->Client_SwitchToMain();
}

///////////////////////////////////////////////////////////////

void UWG_Setting::OnMasterVolumeChanged(float Value)
{
	//#. sound changed
	K_YG_SIMPLE(Warning);
	MasterSoundClass->Properties.Volume = Value;

	//#. value changed
	if (Mypc)
	{
		//#. PlayerState안에 객체에 값 저장하기
		Mypc->GetPlayerState<AK_YG_LobbyPlayerState>()->SetMasterVolume(Value);
	}
}

void UWG_Setting::OnEffectVolumeChanged(float Value)
{
	//#. sound changed
	K_YG_SIMPLE(Warning);
	EffectSoundClass->Properties.Volume = Value;

	//#. value changed
	if (Mypc)
	{
		//#. PlayerState안에 객체에 값 저장하기
		Mypc->GetPlayerState<AK_YG_LobbyPlayerState>()->SetEffectVolume(Value);
	}
}

void UWG_Setting::OnFullScreenChanged()
{
	//#. Screen changed
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();

	if (GameUserSettings)
	{
		// Toggle between fullscreen and windowed fullscreen modes
		EWindowMode::Type CurrentMode = GameUserSettings->GetFullscreenMode();

		if (CurrentMode == EWindowMode::Fullscreen || CurrentMode == EWindowMode::WindowedFullscreen)
		{
			// Switch to windowed mode
			GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
			K_YG_SIMPLE(Warning);
		}
		else
		{
			// Switch to fullscreen mode
			GameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
			K_YG_SIMPLE(Warning);
		}

		// Apply the changes
		GameUserSettings->ApplySettings(false);
	}
	K_YG_SIMPLE(Warning);

	//#. value changed
	if (Mypc)
	{
		//#. PlayerState안에 객체에 값 저장하기
		Mypc->GetPlayerState<AK_YG_LobbyPlayerState>()->SetIsFullScreen(!(CurrentIsFullScreen));
	}
}
