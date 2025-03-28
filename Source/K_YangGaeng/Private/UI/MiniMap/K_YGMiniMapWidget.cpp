// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/MiniMap/K_YGMiniMapWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"

// NativeConstruct 함수
void UK_YGMiniMapWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // MiniMapImage가 유효한지 확인
    K_YG_CHECK(MiniMapImage, );
}

// SetMiniMapTexture 함수
void UK_YGMiniMapWidget::SetMiniMapTexture(UTextureRenderTarget2D* RenderTarget)
{
    // 디버깅 로그 추가
    if (!RenderTarget) {
        UE_LOG(LogTemp, Error, TEXT("SetMiniMapTexture: RenderTarget is nullptr"));
    }

    // MiniMapImage와 RenderTarget이 유효한지 확인
    K_YG_CHECK(MiniMapImage, );
    K_YG_CHECK(RenderTarget, );

    if (!MiniMapMaterialInstance)
    {
        // 지정된 경로에서 Material을 로드
        UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/K_YangGaeng/UI/MiniMap/MiniMapRender_Mat.MiniMapRender_Mat"));
        // Material이 성공적으로 로드되었는지 확인
        K_YG_CHECK(Material, );
        // Material로부터 동적 Material 인스턴스를 생성
        MiniMapMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
        // 동적 Material 인스턴스가 성공적으로 생성되었는지 확인
        K_YG_CHECK(MiniMapMaterialInstance, );
    }
    // MiniMapMaterialInstance가 유효한 경우
    if (MiniMapMaterialInstance)
    {
        // 동적 Material 인스턴스의 텍스처 파라미터를 RenderTarget으로 설정
        MiniMapMaterialInstance->SetTextureParameterValue(FName("MiniMapRendererTarget"), Cast<UTexture>(RenderTarget));
        // MiniMapImage에 동적 Material 인스턴스를 브러시로 설정
        MiniMapImage->SetBrushFromMaterial(MiniMapMaterialInstance);
    }
}
