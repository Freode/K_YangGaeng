// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "K_YGMiniMapWidget.generated.h"

class UImage;
class UMaterialInstanceDynamic;
class UTextureRenderTarget2D;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UK_YGMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// SetMiniMapTexture 미니맵의 텍스처를 설정하는 함수
	// Texture: 미니맵에 적용할 UTexture2D 객체
	// 텍스처를 UImage 컴포넌트의 브러시에 적용하여 화면에 표시
	UFUNCTION(BlueprintCallable, Category = "MiniMap")
	void SetMiniMapTexture(UTextureRenderTarget2D* RenderTarget);

protected:

	// 위젯이 생성될 때 호출
	virtual void NativeConstruct() override;

	// MiniMapImage는 미니맵의 텍스처를 표시할 이미지 컴포넌트
	// 블루프린트에서 이 컴포넌트를 바인딩해 미니맵 렌더링
	UPROPERTY(meta = (BindWidget))
	UImage* MiniMapImage;

private:
	//미니맵의 텍스처를 적용할 동적 Material 인스턴스
	//UMaterialInstanceDynamic을 사용하여 미니맵 텍스처 설정
	UMaterialInstanceDynamic* MiniMapMaterialInstance;

};
