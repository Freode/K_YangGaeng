// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_Setting_Button.generated.h"


class UButton;
class UTextBlock;


/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_Setting_Button : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Select;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Enable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Name;

	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

	UFUNCTION(BlueprintCallable)
	class UButton* GetButton();

	UFUNCTION(BlueprintCallable)
	void SetTitle(FString Title);

	UFUNCTION(BlueprintCallable)
	void OnButtonPressed(bool Value);

	///////////////////////////////////////////////////////////////////////////////

	bool bIsButtonClicked;

protected:

private:

};
