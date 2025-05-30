// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyGameState.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "Engine/GameViewportClient.h"
#include "Blueprint/UserWidget.h"
#include "UI/ScoreBoard/WG_ScoreBoard.h"
#include "UI/MainScreen/WG_Credit.h"
#include "UI/MainScreen/WG_MainScreen.h"
#include "UI/Setting/WG_Setting.h"
#include "UI/Login/WG_Login.h"
#include "UI/Menu/WG_Menu.h"
#include "Components/AudioComponent.h"




void AK_YG_LobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    //#. controller 생성시 마우스 움직임 UI 모드로 변경
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());

    //#. 처음 화면을 Login으로 초기화
    if (bShowMouseCursor) {
        K_YG_SIMPLE(Warning);
        Client_SwitchToLogin();
    }
}

AK_YG_LobbyPlayerController::AK_YG_LobbyPlayerController()
{
    //#. replicated 설정
    bReplicates = true;

    //#. Credit widget class 가져오기
    static ConstructorHelpers::FClassFinder<UUserWidget> CreditWidgetClassFinder(TEXT("/Game/K_YangGaeng/UI/MainScreen/WG_Credit.WG_Credit_C"));
    if (CreditWidgetClassFinder.Succeeded()) {
        CreditWidgetClass = CreditWidgetClassFinder.Class;
    }

    //#. Main widget class 가져오기
    static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetClassFinder(TEXT("/Game/K_YangGaeng/UI/MainScreen/WG_MainScreen.WG_MainScreen_C"));
    if (MainWidgetClassFinder.Succeeded()) {
        MainWidgetClass = MainWidgetClassFinder.Class;
    }

    //#. Login widget class 가져오기
    static ConstructorHelpers::FClassFinder<UUserWidget> LoginWidgetClassFinder(TEXT("/Game/K_YangGaeng/UI/Login/BP_CreateChar.BP_CreateChar_C"));
    if (LoginWidgetClassFinder.Succeeded())
    {
        K_YG_SIMPLE(Warning);
        LoginWidgetClass = LoginWidgetClassFinder.Class;
    }

    //#. Setting widget class 가져오기
    static ConstructorHelpers::FClassFinder<UUserWidget> SettingWidgetClassFinder(TEXT("/Game/K_YangGaeng/UI/Setting/WG_Setting.WG_Setting_C"));
    if (SettingWidgetClassFinder.Succeeded())
    {
        K_YG_SIMPLE(Warning);
        SettingWidgetClass = SettingWidgetClassFinder.Class;
    }

    //#. Menu widget class 가져오기
    static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetClassFinder(TEXT("/Game/K_YangGaeng/UI/Menu/WG_Menu.WG_Menu_C"));
    if (MenuWidgetClassFinder.Succeeded())
    {
        K_YG_SIMPLE(Warning);
        MenuWidgetClass = MenuWidgetClassFinder.Class;
    }

    //#. scoreboard widget class 가져오기
    static ConstructorHelpers::FClassFinder<UUserWidget> ScoreBoardWidgetClassFinder(TEXT("/Game/K_YangGaeng/UI/ScoreBoard/WG_ScoreBoard.WG_ScoreBoard_C"));
    if (ScoreBoardWidgetClassFinder.Succeeded())
    {
        K_YG_SIMPLE(Warning);
        ScoreBoardWidgetClass = ScoreBoardWidgetClassFinder.Class;
    }

    //#. CurrentWidget 초기화
    CurrentWidget = nullptr;
}

void AK_YG_LobbyPlayerController::SetSoundVolume(float Volume)
{
    //if (MyAudioComponent)
    //{
    //    MyAudioComponent->SetVolumeMultiplier(Volume);
    //    UE_LOG(LogTemp, Warning, TEXT("Volume set to: %f"), Volume);
    //}
}

void AK_YG_LobbyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    //#. F key binding
    InputComponent->BindKey(EKeys::F, IE_Pressed, this, &AK_YG_LobbyPlayerController::OpenMenuWidget);
}

void AK_YG_LobbyPlayerController::Client_SwitchToMain_Implementation()
{
    if (IsLocalController() && MainWidgetClass)
    {
        //#. 현재 올라와 있는 widget 제거
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr;
        }

        //#. 메인 widget 불러오기
        CurrentWidget = CreateWidget<UWG_MainScreen>(this, MainWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}
                                  
void AK_YG_LobbyPlayerController::Client_SwitchToLogin_Implementation()
{
    if (IsLocalController() && LoginWidgetClass)
    {
        //#. 현재 올라와 있는 widget 제거
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr;
        }

        //#. Gameplay widget 불러오기
        CurrentWidget = CreateWidget<UWG_Login>(this, LoginWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}
                                  
void AK_YG_LobbyPlayerController::Client_SwitchToCredit_Implementation()
{
    if (IsLocalController() && CreditWidgetClass)
    {
        //#. 현재 올라와 있는 widget 제거
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr;
        }

        //#. Gameplay widget 불러오기
        CurrentWidget = CreateWidget<UWG_Credit>(this, CreditWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}
                                  
void AK_YG_LobbyPlayerController::Client_SwitchToStart_Implementation()
{
    //#. 현재 올라와 있는 widget 제거
    if (IsLocalController() && CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    //#. 플레이어 인풋 모드 변경
    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());

}
                                  
void AK_YG_LobbyPlayerController::Client_SwitchToSetting_Implementation()
{
    if (IsLocalController() && SettingWidgetClass)
    {
        //#. 현재 올라와 있는 widget 제거
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr;
        }

        //#. Setting widget 불러오기
        CurrentWidget = CreateWidget<UWG_Setting>(this, SettingWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

void AK_YG_LobbyPlayerController::OpenMenuWidget()
{
    //#. login 한 경우에만 열리도록 설정
    bool bIsLoggedIn = GetPlayerState<AK_YG_LobbyPlayerState>()->bIsLoggedIn;

    K_YG_SIMPLE(Warning);
    if (IsLocalController() && ScoreBoardWidgetClass && bIsLoggedIn)
    {
        //#. 현재 올라와 있는 widget 제거
        K_YG_SIMPLE(Warning);

        CurrentWidget = CreateWidget<UUserWidget>(this, ScoreBoardWidgetClass);
        if (CurrentWidget)
        {
            K_YG_SIMPLE(Warning);
            CurrentWidget->AddToViewport();

            CurrentWidget->SetKeyboardFocus();
            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());

            //#. 현재는 Login 시에 이미 색깔이 정해지므로
            FString TeamColor = GetPlayerState<AK_YG_LobbyPlayerState>()->GetPlayerTeamColor();
            UWG_ScoreBoard* CastedCurrentWidget = Cast<UWG_ScoreBoard>(CurrentWidget);

            CastedCurrentWidget->ChangeBorderColor(TeamColor);


            //CurrentWidget->...?
            //bIsOpenMenu = true;
        }
    }
}

//#. 서버 함수 실행하는법(controller에서 gamestate부르기)
//#. WG_Login 쪽에서 실행 될 예정
//#. 왜 WG에서 바로 실행하지 않을까...? -> 아마 컨트롤러에서 불러야 서버 입장에서 누가 불렀는지 알 수 있기 때문일 듯




void AK_YG_LobbyPlayerController::ServerNameChecked_Implementation(const FString& NickName)
{
    GetWorld()->GetGameState<AK_YG_LobbyGameState>()->AddPlayerName(this, NickName);
}

void AK_YG_LobbyPlayerController::ClientNamePassed_Implementation(const FString& NickName)
{
    //#. client에서 실행할 코드를 여기 적자
    UE_LOG(LogTemp, Log, TEXT("Passed Name: %s"), *NickName);

    //#. 로그인 성공 -> 위젯에서 생성버튼 활성화 & 확인버튼 비활성화
    UWG_Login* CastedCurrentWidget = Cast<UWG_Login>(CurrentWidget);
    CastedCurrentWidget->DuplicationPassed();
    CastedCurrentWidget->SetTitleSucceed();

    //#. PlayerState에 내용 전달 -> 후에 다른 이름으로 로그인 하지 못하도록 설정
    GetPlayerState<AK_YG_LobbyPlayerState>()->Login(NickName);
    
}

void AK_YG_LobbyPlayerController::ClientNameUnPassed_Implementation(const FString& NickName)
{
    UE_LOG(LogTemp, Log, TEXT("UnPassed Name: %s"), *NickName);

    //#. 로그인 성공 -> 위젯에서 생성버튼 활성화 & 확인버튼 비활성화
    UWG_Login* CastedCurrentWidget = Cast<UWG_Login>(CurrentWidget);
    CastedCurrentWidget->DuplicationUnPassed();
    CastedCurrentWidget->SetTitleFailed();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#. 유효성 검사 함수

bool AK_YG_LobbyPlayerController::ServerNameChecked_Validate(const FString& NickName)
{
    //#. 이름이 비워져있지만 않으면 true반환
    return !NickName.IsEmpty();
}

bool AK_YG_LobbyPlayerController::ClientNamePassed_Validate(const FString& NickName)
{
    return true;
}

bool AK_YG_LobbyPlayerController::ClientNameUnPassed_Validate(const FString& NickName)
{
    return true;
}

bool AK_YG_LobbyPlayerController::Client_SwitchToMain_Validate()
{
    return true;
}

bool AK_YG_LobbyPlayerController::Client_SwitchToLogin_Validate()
{
    return true;
}

bool AK_YG_LobbyPlayerController::Client_SwitchToCredit_Validate()
{
    return true;
}

bool AK_YG_LobbyPlayerController::Client_SwitchToStart_Validate()
{
    return true;
}

bool AK_YG_LobbyPlayerController::Client_SwitchToSetting_Validate()
{
    return true;
}




