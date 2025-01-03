// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_Setting_Slide.generated.h"


class USlider;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_Setting_Slide : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USlider* Slider_Select;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Slide_Persentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Slide_Select;

	///////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable)
	void OnSlideChanged(float Value);

	UFUNCTION(BlueprintCallable)
	class USlider* GetSlider();

	UFUNCTION(BlueprintCallable)
	void SetTitle(FString Title);
};
