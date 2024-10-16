// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/WorldMap/K_YGWorldMapWidget.h"
#include "UI/WorldMap/WorldMapComponent.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// NativeConstruct 함수
void UK_YGWorldMapWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // MiniMapImage가 유효한지 확인
    K_YG_CHECK(WorldMapImage, );
}

FReply UK_YGWorldMapWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    const float ScrollDelta = InMouseEvent.GetWheelDelta();
    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if (PlayerController)
        {
            APawn* Pawn = PlayerController->GetPawn();
            if (Pawn)
            {
                UWorldMapComponent* WorldMapComponent = Pawn->FindComponentByClass<UWorldMapComponent>();
                if (WorldMapComponent)
                {
                    WorldMapComponent->AdjustOrthoWidth(ScrollDelta);
                }
            }
        }
    }

    return FReply::Handled();
}

// SetMiniMapTexture 함수
void UK_YGWorldMapWidget::SetWorldMapTexture(UTextureRenderTarget2D* RenderTarget)
{
    // 디버깅 로그 추가
    if (!RenderTarget) {
        UE_LOG(LogTemp, Error, TEXT("SetWorldMapTexture: RenderTarget is nullptr"));
    }

    // MiniMapImage와 RenderTarget이 유효한지 확인
    K_YG_CHECK(WorldMapImage, );
    K_YG_CHECK(RenderTarget, );

    if (!WorldMapMaterialInstance)
    {
        // 지정된 경로에서 Material을 로드
        UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/K_YangGaeng/UI/WorldMap/WorldMapRender_Mat.WorldMapRender_Mat"));
        // Material이 성공적으로 로드되었는지 확인
        K_YG_CHECK(Material, );
        // Material로부터 동적 Material 인스턴스를 생성
        WorldMapMaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
        // 동적 Material 인스턴스가 성공적으로 생성되었는지 확인
        K_YG_CHECK(WorldMapMaterialInstance, );
    }
    // MiniMapMaterialInstance가 유효한 경우
    if (WorldMapMaterialInstance)
    {
        // 동적 Material 인스턴스의 텍스처 파라미터를 RenderTarget으로 설정
        WorldMapMaterialInstance->SetTextureParameterValue(FName("WorldMapMapRendererTarget"), Cast<UTexture>(RenderTarget));
        // MiniMapImage에 동적 Material 인스턴스를 브러시로 설정
        WorldMapImage->SetBrushFromMaterial(WorldMapMaterialInstance);
    }
}