// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#pragma once

#include "../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h" 
#include "InputCoreTypes.h" 
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "K_YGWorldMapWidget.generated.h"

class UTextureRenderTarget2D;
class UImage;
class UMaterialInstanceDynamic;

/**
 *
 */
UCLASS()
class K_YANGGAENG_API UK_YGWorldMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 월드맵의 텍스처를 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "WorldMap")
	void SetWorldMapTexture(UTextureRenderTarget2D* RenderTarget);

protected:
	// 위젯이 생성될 때 호출
	virtual void NativeConstruct() override;

	// 마우스 휠 스크롤 처리 함수
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// MiniMapImage는 미니맵의 텍스처를 표시할 이미지 컴포넌트
	UPROPERTY(meta = (BindWidget))
	UImage* WorldMapImage;

private:
	// 월드맵의 텍스처를 적용할 동적 Material 인스턴스
	UMaterialInstanceDynamic* WorldMapMaterialInstance;

	bool bIsDragging = false;
};
