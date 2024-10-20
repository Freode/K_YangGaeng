// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/MiniMap/MiniMapComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "UI/MiniMap/K_YGMiniMapWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

// 기본 생성자
UMiniMapComponent::UMiniMapComponent()
{
    // Tick을 활성화하여 매 프레임 업데이트
    PrimaryComponentTick.bCanEverTick = true;

    // 컴포넌트 초기화
    InitializeComponents();
}

void UMiniMapComponent::InitializeComponents()
{
    // SpringArmComponent를 생성
    MiniMapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapSpringArm"));

    // SpringArm의 길이 설정
    MiniMapSpringArm->TargetArmLength = 2000.f;

    // SpringArm을 수직으로 회전시켜 미니맵 캡처 컴포넌트가 상향으로 바라보도록 설정
    MiniMapSpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

    // SceneCaptureComponent2D를 생성하고 SpringArm에 부착
    MiniMapCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCaptureComponent"));
    MiniMapCaptureComponent->SetupAttachment(MiniMapSpringArm);
    MiniMapCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;

    // Orthographic width를 설정하여 시야를 넓게 함
    MiniMapCaptureComponent->OrthoWidth = 2048.0f;

    // Blueprint에서 미니맵 위젯 클래스를 로드
    static ConstructorHelpers::FClassFinder<UUserWidget> MiniMapWidgetBPClass(TEXT("/Game/K_YangGaeng/UI/MiniMap/WG_MiniMapWidget.WG_MiniMapWidget_C"));
    if (MiniMapWidgetBPClass.Succeeded())
    {
        MiniMapWidgetClass = MiniMapWidgetBPClass.Class;
    }

    // 레퍼런스 경로에서 RenderTarget 로드
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MiniMapRenderTargetObj(TEXT("/Game/K_YangGaeng/UI/MiniMap/MiniMapRender.MiniMapRender"));
    if (MiniMapRenderTargetObj.Succeeded())
    {
        MiniMapRenderTarget = MiniMapRenderTargetObj.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MiniMapRenderTarget 로드 실패"));
    }

    // MiniMapCaptureComponent의 TextureTarget을 설정
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MiniMapRenderObj(TEXT("/Game/K_YangGaeng/UI/MiniMap/MiniMapRender.MiniMapRender"));
    if (MiniMapRenderObj.Succeeded())
    {
        MiniMapCaptureComponent->TextureTarget = MiniMapRenderObj.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MiniMapRenderObj 로드 실패"));
    }

    // Arrow_Sprite를 생성하고 SpringArm에 부착
    ArrowSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ArrowSprite"));
    ArrowSprite->SetupAttachment(MiniMapSpringArm);

    // Arrow_Sprite의 스프라이트 설정
    static ConstructorHelpers::FObjectFinder<UPaperSprite> ArrowSpriteObj(TEXT("/Game/K_YangGaeng/UI/MiniMap/Arrow_Sprite.Arrow_Sprite"));
    if (ArrowSpriteObj.Succeeded())
    {
        ArrowSprite->SetSprite(ArrowSpriteObj.Object);

        // ArrowSprite를 수평 방향으로 눕히기 위해 Z축을 기준으로 90도 회전
        ArrowSprite->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
    }

    // 미니맵에서 플레이어 위로 약간 올리기
    ArrowSprite->SetRelativeLocation(FVector(850.f, 0.f, 0.f));
    // 크기 조절
    ArrowSprite->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.55f));
    // MiniMapCaptureComponent에서만 보이도록 하기
    ArrowSprite->bVisibleInSceneCaptureOnly = true;
}

void UMiniMapComponent::BeginPlay()
{
    Super::BeginPlay();

    // 소유자가 유효한지 확인하고, 소유자가 AActor인지 확인
    if (AActor* Owner = GetOwner())
    {
        // 소유자의 루트 컴포넌트에 SpringArmComponent를 부착
        if (USceneComponent* RootComponent = Owner->GetRootComponent())
        {
            MiniMapSpringArm->SetupAttachment(RootComponent);
        }
    }

    // SceneCaptureComponent2D를 활성화해 미니맵의 장면 캡처 시작
    if (MiniMapCaptureComponent)
    {
        MiniMapCaptureComponent->Activate(true);
    }

    // ArrowSprite를 활성화
    if (ArrowSprite)
    {
        ArrowSprite->SetVisibility(true);
    }

    // 미니맵 위젯을 생성 후 뷰포트에 추가
    if (MiniMapWidgetClass)
    {
        UUserWidget* MiniMapWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MiniMapWidgetClass);
        if (MiniMapWidgetInstance)
        {
            MiniMapWidgetInstance->AddToViewport();

            // 위젯을 UK_YGMiniMapWidget으로 캐스팅
            MiniMapWidget = Cast<UK_YGMiniMapWidget>(MiniMapWidgetInstance);
            if (MiniMapWidget)
            {
                // RenderTarget을 UTextureRenderTarget2D로 설정하여 미니맵 위젯에 설정
                if (MiniMapRenderTarget) {
                    MiniMapWidget->SetMiniMapTexture(MiniMapRenderTarget);
                }
                else {
                    UE_LOG(LogTemp, Error, TEXT("MiniMapRenderTarget is nullptr"));
                }
            }
        }
    }
}

void UMiniMapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (AActor* Owner = GetOwner())
    {
        // 소유자의 위치와 회전을 따라가도록 업데이트
        FVector OwnerLocation = Owner->GetActorLocation();
        FRotator OwnerRotation = Owner->GetActorRotation();

        MiniMapSpringArm->SetWorldLocation(OwnerLocation);
        MiniMapSpringArm->SetWorldRotation(FRotator(-90.f, OwnerRotation.Yaw, 0.f));
    }
}
