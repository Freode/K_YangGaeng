// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_MainScreen.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_MainScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_GameStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Setting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Credit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Exit;

	//#. Game 서버로 이동 함수 생성
	UFUNCTION(BlueprintCallable)
	void OnGameStartButtonClicked();

	//#. Setting 화면으로 이동 함수 생성
	UFUNCTION(BlueprintCallable)
	void OnSettingButtonClicked();

	//#. Credit 화면 이동 함수 생성
	UFUNCTION(BlueprintCallable)
	void OnCreditButtonClicked();
	
	//#. 게임종료 함수 생성
	UFUNCTION(BlueprintCallable)
	void OnExitButtonClicked();

private:
	//#. 서버에서 관리할 함수를 여기다 넣을 예정
};
