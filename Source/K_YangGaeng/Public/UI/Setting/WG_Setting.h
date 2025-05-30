// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_Setting.generated.h"

class UButton;
class USlider;
class UScrollBox;
class USoundClass;
class UWG_Setting_Slide;
class UWG_Setting_Button;
class AK_YG_LobbyPlayerController;



/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_Setting : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	//#. main 화면으로 돌아가는 버튼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Back;

	//#. 스크롤 박스(세팅할 내용이 들어있는 widget)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UScrollBox* ScrollBox_Setting;

	///////////////////////////////////////////////////////////////////////////////

	//#. MasterVolume 조절 widget(blueprint binding)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWG_Setting_Slide* MasterVolumeWidget;

	//#. EffectVolume 조절 widget(blueprint binding)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWG_Setting_Slide* EffectVolumeWidget;

	//#. isFullScreen 조절 widget(blueprint binding)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWG_Setting_Button* FullScreenWidget;

	///////////////////////////////////////////////////////////////////////////////

	//#. MasterVolume 관할 클래스 가져오기 (in blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundClass* MasterSoundClass;

	//#. EffectVolume 관할 클래스 가져오기 (in blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundClass* EffectSoundClass;

	///////////////////////////////////////////////////////////////////////////////

	//#. Main 화면으로 다시 이동 함수 생성
	UFUNCTION(BlueprintCallable)
	void OnBackButtonClicked();

	///////////////////////////////////////////////////////////////////////////////


	//#. MasterVolumeSlider 값 변환에 따른 함수
	UFUNCTION(BlueprintCallable)
	void OnMasterVolumeChanged(float Value);

	//#. EffectVolumeSlider 값 변환에 따른 함수
	UFUNCTION(BlueprintCallable)
	void OnEffectVolumeChanged(float Value);

	//#. IsFullScreen 값 변환에 따른 함수
	UFUNCTION(BlueprintCallable)
	void OnFullScreenChanged();
	
	///////////////////////////////////////////////////////////////////////////////

	class AK_YG_LobbyPlayerController* Mypc;

	float CurrentMasterVolume;

	float CurrentEffectVolume;

	bool CurrentIsFullScreen;
};
