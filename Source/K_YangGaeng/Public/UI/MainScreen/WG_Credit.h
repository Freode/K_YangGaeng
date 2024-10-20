// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_Credit.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_Credit : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Back;

	//#. Main 화면으로 다시 이동 함수 생성
	UFUNCTION(BlueprintCallable)
	void OnBackButtonClicked();

private:
};
