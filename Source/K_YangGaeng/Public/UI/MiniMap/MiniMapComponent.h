// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Components/SceneComponent.h"
#include "MiniMapComponent.generated.h"

class USpringArmComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class UUserWidget;
class UK_YGMiniMapWidget;
class UPaperSpriteComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class K_YANGGAENG_API UMiniMapComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // 기본 생성자
    UMiniMapComponent();

protected:
    // 게임 시작 시 호출
    virtual void BeginPlay() override;

    // 매 프레임마다 호출
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // MiniMapSpringArm: 미니맵을 위한 스프링 암 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* MiniMapSpringArm;

    // MiniMapCaptureComponent: 미니맵을 캡처하기 위한 씬 캡처 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    USceneCaptureComponent2D* MiniMapCaptureComponent;

    // MiniMapRenderTarget: 미니맵의 이미지를 저장할 렌더 타겟 텍스처
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UTextureRenderTarget2D* MiniMapRenderTarget;

    // MiniMapWidgetClass: 미니맵 위젯의 클래스
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> MiniMapWidgetClass;

    // MiniMapWidget: 생성된 미니맵 위젯 인스턴스
    UPROPERTY()
    UK_YGMiniMapWidget* MiniMapWidget;

    // ArrowSprite: 플레이어의 위에 표시할 화살표 스프라이트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UPaperSpriteComponent* ArrowSprite;

    // 컴포넌트들을 설정하는 함수
    void InitializeComponents();
};
