// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/WorldMap/WorldMapComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "UI/WorldMap/K_YGWorldMapWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"

UWorldMapComponent::UWorldMapComponent()
{
    // Tick을 활성화하여 매 프레임 업데이트
    PrimaryComponentTick.bCanEverTick = true;

    // 컴포넌트 초기화
    InitializeComponents();

    // 드래그 관련 변수 초기화
    bIsDragging = false;
}

void UWorldMapComponent::InitializeComponents()
{
    // SpringArmComponent를 생성
    WorldMapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("WorldMapSpringArm"));

    // SpringArm의 길이 설정
    WorldMapSpringArm->TargetArmLength = 2000.f;

    // SpringArm을 수직으로 회전시켜 미니맵 캡처 컴포넌트가 상향으로 바라보도록 설정
    WorldMapSpringArm->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

    // SceneCaptureComponent2D를 생성하고 SpringArm에 부착
    WorldMapCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("WorldMapCaptureComponent"));
    WorldMapCaptureComponent->SetupAttachment(WorldMapSpringArm);
    WorldMapCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;

    // Orthographic width를 설정하여 시야를 넓게 함
    WorldMapCaptureComponent->OrthoWidth = 5500.0f;

    // Blueprint에서 미니맵 위젯 클래스를 로드
    static ConstructorHelpers::FClassFinder<UUserWidget> WorldMapWidgetBPClass(TEXT("/Game/K_YangGaeng/UI/WorldMap/WG_WorldMapWidget.WG_WorldMapWidget_C"));
    if (WorldMapWidgetBPClass.Succeeded())
    {
        WorldMapWidgetClass = WorldMapWidgetBPClass.Class;
    }

    // 레퍼런스 경로에서 RenderTarget 로드
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> WorldMapRenderTargetObj(TEXT("/Game/K_YangGaeng/UI/WorldMap/WorldMapRender.WorldMapRender"));
    if (WorldMapRenderTargetObj.Succeeded())
    {
        WorldMapRenderTarget = WorldMapRenderTargetObj.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WorldMapRenderTarget 로드 실패"));
    }

    // WorldMapCaptureComponent의 TextureTarget을 설정
    static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> WorldMapRenderObj(TEXT("/Game/K_YangGaeng/UI/WorldMap/WorldMapRender.WorldMapRender"));
    if (WorldMapRenderObj.Succeeded())
    {
        WorldMapCaptureComponent->TextureTarget = WorldMapRenderObj.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("WorldMapRenderObj 로드 실패"));
    }

    // WorldMapPing_Sprite를 생성하고 SpringArm에 부착
    WorldMapPing = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("WorldMapPing"));
    WorldMapPing->SetupAttachment(WorldMapSpringArm);

    // WorldMapPing_Sprite의 스프라이트 설정
    static ConstructorHelpers::FObjectFinder<UPaperSprite> WorldMapPingSpriteObj(TEXT("/Game/K_YangGaeng/UI/WorldMap/WorldMapPing_Sprite.WorldMapPing_Sprite"));
    if (WorldMapPingSpriteObj.Succeeded())
    {
        WorldMapPing->SetSprite(WorldMapPingSpriteObj.Object);

        // WorldMapPingSprite를 수평 방향으로 눕히기 위해 Z축을 기준으로 90도 회전
        WorldMapPing->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
    }

    // 미니맵에서 플레이어 위로 약간 올리기
    WorldMapPing->SetRelativeLocation(FVector(850.f, 0.f, 0.f));
    // 크기 조절
    WorldMapPing->SetRelativeScale3D(FVector(0.55f, 0.55f, 0.55f));
    // MiniMapCaptureComponent에서만 보이도록 하기
    WorldMapPing->bVisibleInSceneCaptureOnly = true;

    // 미니맵 위젯 초기 가시성을 false로 설정
    bIsWorldMapVisible = false;
}

void UWorldMapComponent::BeginPlay()
{
    Super::BeginPlay();

    // 소유자가 유효한지 확인하고, 소유자가 AActor인지 확인
    if (AActor* Owner = GetOwner())
    {
        // 소유자의 루트 컴포넌트에 SpringArmComponent를 부착
        if (USceneComponent* RootComponent = Owner->GetRootComponent())
        {
            WorldMapSpringArm->SetupAttachment(RootComponent);
        }

        // 소유자가 Pawn 또는 Character인지 확인하여 플레이어 컨트롤러에서 입력 컴포넌트를 가져옴
        if (APawn* OwnerPawn = Cast<APawn>(Owner))
        {
            if (APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController()))
            {
                if (UInputComponent* InputComponent = PlayerController->InputComponent)
                {
                    InputComponent->BindAction("WorldMap", IE_Pressed, this, &UWorldMapComponent::ToggleWorldMapVisibility);
                    InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &UWorldMapComponent::OnLeftMouseButtonDown);
                    InputComponent->BindAction("LeftMouseButton", IE_Released, this, &UWorldMapComponent::OnLeftMouseButtonUp);
                    InputComponent->BindAxis("MouseX", this, &UWorldMapComponent::OnMouseMoveX);
                    InputComponent->BindAxis("MouseY", this, &UWorldMapComponent::OnMouseMoveY);
                    InputComponent->BindAxis("MouseWheel", this, &UWorldMapComponent::AdjustOrthoWidth);
                }
            }
        }
    }

    // SceneCaptureComponent2D를 활성화해 미니맵의 장면 캡처 시작
    if (WorldMapCaptureComponent)
    {
        WorldMapCaptureComponent->Activate(true);
    }

    // WorldMapPingSprite를 활성화
    if (WorldMapPing)
    {
        WorldMapPing->SetVisibility(true);
    }

    // 미니맵 위젯을 생성 후 뷰포트에 추가
    if (WorldMapWidgetClass)
    {
        WorldMapWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WorldMapWidgetClass);
        if (WorldMapWidgetInstance)
        {
            WorldMapWidgetInstance->AddToViewport();

            // 위젯을 UK_YGWorldMapWidget으로 캐스팅
            WorldMapWidget = Cast<UK_YGWorldMapWidget>(WorldMapWidgetInstance);
            if (WorldMapWidget)
            {
                // RenderTarget을 UTextureRenderTarget2D로 설정하여 미니맵 위젯에 설정
                if (WorldMapRenderTarget)
                {
                    WorldMapWidget->SetWorldMapTexture(WorldMapRenderTarget);
                }
                // 초기 가시성을 숨김으로 설정
                WorldMapWidget->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
}

void UWorldMapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 드래그 중일 때 마우스 이동에 따른 SpringArm 위치 업데이트
    if (bIsDragging)
    {
        FVector2D CurrentMousePosition;
        if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);

            FVector2D MouseDelta = CurrentMousePosition - InitialMousePosition;

            FVector NewLocation = InitialSpringArmLocation;
            NewLocation.X -= MouseDelta.X * 5.0f; // 이동 속도 조절
            NewLocation.Y += MouseDelta.Y * 5.0f; // 이동 속도 조절

            WorldMapSpringArm->SetWorldLocation(NewLocation);
        }
    }

    // 소유자의 위치를 따라가도록 업데이트
    if (AActor* Owner = GetOwner())
    {
        FVector OwnerLocation = Owner->GetActorLocation();

        // 회전을 고정된 값으로 설정 (예: 상향을 바라보도록 설정)
        FRotator FixedRotation = FRotator(-90.f, 0.f, 0.f);

        WorldMapSpringArm->SetWorldLocation(OwnerLocation);
        WorldMapSpringArm->SetWorldRotation(FixedRotation);
    }
}

void UWorldMapComponent::ToggleWorldMapVisibility()
{
    if (WorldMapWidgetInstance)
    {
        bIsWorldMapVisible = !bIsWorldMapVisible;

        if (bIsWorldMapVisible) {
            // 위젯을 표시
            WorldMapWidgetInstance->SetVisibility(ESlateVisibility::Visible);

            // 마우스 커서 활성화
            ToggleMouseCursor(true);
        }
        else
        {
            // 위젯을 숨김
            WorldMapWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
            // 마우스 커서 비활성화
            ToggleMouseCursor(false);
        }
    }
}

void UWorldMapComponent::ToggleMouseCursor(bool bShowCursor)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController != nullptr)
    {
        // 마우스 커서 표시/숨김 설정
        PlayerController->bShowMouseCursor = bShowCursor;
        PlayerController->bEnableClickEvents = bShowCursor;
        PlayerController->bEnableMouseOverEvents = bShowCursor;

        if (bShowCursor)
        {
            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputMode.SetWidgetToFocus(WorldMapWidgetInstance->TakeWidget());
            PlayerController->SetInputMode(InputMode);
        }
        else
        {
            PlayerController->SetInputMode(FInputModeGameOnly());
        }
    }
}

void UWorldMapComponent::AdjustOrthoWidth(float ScrollDelta)
{
    if (FMath::IsNearlyZero(ScrollDelta))
        return;

    if (WorldMapCaptureComponent)
    {
        const float ZoomSpeed = 500.0f;
        float NewOrthoWidth = WorldMapCaptureComponent->OrthoWidth + ScrollDelta * ZoomSpeed;
        WorldMapCaptureComponent->OrthoWidth = FMath::Clamp(NewOrthoWidth, 1000.0f, 10000.0f);
    }
}

void UWorldMapComponent::OnLeftMouseButtonDown()
{
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerController->GetMousePosition(InitialMousePosition.X, InitialMousePosition.Y);
        InitialSpringArmLocation = WorldMapSpringArm->GetComponentLocation();
        bIsDragging = true;
    }
}

void UWorldMapComponent::OnLeftMouseButtonUp()
{
    bIsDragging = false;
}

void UWorldMapComponent::OnMouseMoveX(float AxisValue)
{
    if (bIsDragging && FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
    {
        FVector NewLocation = InitialSpringArmLocation;
        NewLocation.X -= AxisValue * 5.0f; // 이동 속도 조절
        WorldMapSpringArm->SetWorldLocation(NewLocation);
    }
}

void UWorldMapComponent::OnMouseMoveY(float AxisValue)
{
    if (bIsDragging && FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
    {
        FVector NewLocation = InitialSpringArmLocation;
        NewLocation.Y += AxisValue * 5.0f; // 이동 속도 조절
        WorldMapSpringArm->SetWorldLocation(NewLocation);
    }
}
