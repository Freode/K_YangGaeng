// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_Menu.generated.h"

class UButton;
class UImage;
class FReply;
class AK_YG_LobbyPlayerController;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_Menu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	//#. index : 0 , Play Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Play;

	//#. index : 1 , Graphic Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Graphic;

	//#. index : 2 , Audio Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Audio;

	//#. index : 3 , Quit Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Quit;

	//#. Menu Image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Menu;

	UFUNCTION(BlueprintCallable)
	void OnPlayButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnGraphicButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnAudioButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnQuitButtonClicked();

	class AK_YG_LobbyPlayerController* Mypc;

	virtual bool IsFocusable() const
	{
		return true;
	}

protected:

	//#. Widget에서도 키 입력을 받을 수 있도록 setting
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InputKeyEvent) override;
private:
};
