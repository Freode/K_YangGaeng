// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_Login.generated.h"


//#. 전방 선언
class UEditableTextBox;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_Login : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (BindWidget))
	class UEditableTextBox* UserNameTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (BindWidget))
	class UButton* Button_Duplicate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_CreateChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (BindWidget))
	class UTextBlock* StatusTextBlock;

	//#. Check button 클릭 시 함수 생성
	UFUNCTION(BlueprintCallable)
	void OnNameCheckedButtonClicked();

	//#. Create button 클릭 시 함수 생성
	UFUNCTION(BlueprintCallable)
	void OnCreateCharButtonClicked();

	UFUNCTION(BlueprintCallable)
	void DuplicationPassed();

	UFUNCTION(BlueprintCallable)
	void DuplicationUnPassed();

	UFUNCTION()
	void SetTitleSucceed();
	
	UFUNCTION()
	void SetTitleFailed();
};
