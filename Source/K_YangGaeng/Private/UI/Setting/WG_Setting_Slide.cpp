// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/Setting/WG_Setting_Slide.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

void UWG_Setting_Slide::NativeConstruct()
{
	Super::NativeConstruct();

	if (Slider_Select) {
		Slider_Select->OnValueChanged.AddDynamic(this, &UWG_Setting_Slide::OnSlideChanged);
	}
}

//#. slide value 변경될 시 함수 실행
//#. Widget의 크기만을 여기서 설정
//#. 기능 구현은 Setting에서 구현
void UWG_Setting_Slide::OnSlideChanged(float Value)
{
	//#. String 형으로 변환 후 SetText
	FString PercentageString = FString::Printf(TEXT("%.2f%%"), Value * 100.0f);
	TextBlock_Slide_Persentage->SetText(FText::FromString(PercentageString));
	K_YG_SIMPLE(Warning);

	//#. Select 이미지 크기 조정
	if (Image_Slide_Select) {
		//#. image의 slot을 연결해 크기 조절
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Slide_Select->Slot);
		if (CanvasSlot) {
			CanvasSlot->SetSize(FVector2D(Value * 550, 100.0f));
		}
	}
}

USlider* UWG_Setting_Slide::GetSlider()
{
	return Slider_Select;
}

void UWG_Setting_Slide::SetTitle(FString Title)
{
	TextBlock_Name->SetText(FText::FromString(Title));
}
