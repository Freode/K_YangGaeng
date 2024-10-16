// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Components/SceneComponent.h"
#include "WorldMapComponent.generated.h"

class USpringArmComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class UUserWidget;
class UK_YGWorldMapWidget;
class UPaperSpriteComponent;
class UInputComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class K_YANGGAENG_API UWorldMapComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // 기본 생성자
    UWorldMapComponent();

protected:
    // 게임 시작 시 호출
    virtual void BeginPlay() override;

public:
    // 매 프레임 마다 호출
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 월드 맵 위젯의 가시성을 토글하는 함수
    void ToggleWorldMapVisibility();

    // 마우스의 포커스를 설정하는 함수
    void ToggleMouseCursor(bool bShowCursor);

    // 마우스 휠 입력을 처리하는 함수
    void AdjustOrthoWidth(float ScrollDelta);

    // 마우스 클릭 이벤트 처리 함수
    void OnLeftMouseButtonDown();

    // 마우스 버튼 릴리즈 이벤트 처리 함수
    void OnLeftMouseButtonUp();

    // 마우스 이동 이벤트 처리 함수
    void OnMouseMoveX(float AxisValue);
    void OnMouseMoveY(float AxisValue);

private:
    // WorldMapSpringArm: 미니맵을 위한 스프링 암 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* WorldMapSpringArm;

    // WorldMapCaptureComponent: 미니맵을 캡처하기 위한 씬 캡처 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    USceneCaptureComponent2D* WorldMapCaptureComponent;

    // WorldMapRenderTarget: 미니맵의 이미지를 저장할 렌더 타겟 텍스처
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UTextureRenderTarget2D* WorldMapRenderTarget;

    // WorldMapWidgetClass: 미니맵 위젯의 클래스
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> WorldMapWidgetClass;

    // WorldMapWidget: 생성된 미니맵 위젯 인스턴스
    UPROPERTY()
    UK_YGWorldMapWidget* WorldMapWidget;

    //
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UUserWidget* WorldMapWidgetInstance;

    // ArrowSprite: 플레이어의 위에 표시할 화살표 스프라이트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UPaperSpriteComponent* WorldMapPing;

    // 컴포넌트들을 설정하는 함수
    void InitializeComponents();

    // 월드 맵 위젯의 가시성 상태를 추적하는 변수
    bool bIsWorldMapVisible;

    // 마우스 드래그 상태를 추적하는 변수
    bool bIsDragging;

    // 마우스 드래그 시작 시의 초기 위치를 저장하는 변수
    FVector2D InitialMousePosition;

    // 마우스 드래그 시작 시의 SpringArm 위치를 저장하는 변수
    FVector InitialSpringArmLocation;
};
