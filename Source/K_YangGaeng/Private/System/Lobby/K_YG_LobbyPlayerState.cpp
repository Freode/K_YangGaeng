// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "System/Lobby/K_YG_LobbyPlayerState.h"

AK_YG_LobbyPlayerState::AK_YG_LobbyPlayerState()
{

    //#. initialize
	bIsLoggedIn = false;

    //#. 나중에는 bool이 아니라 EWindowMode 로 넘겨야겠다
    bIsFullScreen = true;
    MasterVolume = 0.5f;
    EffectVolume = 0.5f;
}

void AK_YG_LobbyPlayerState::Login(const FString& InPlayerName)
{
    K_YG_SIMPLE(Warning);
    if (InPlayerName == "asdf") {
        //#. 임의로 이름에 따라 색깔 지정
        PlayerInGameInfo.TeamColor = "Blue";
    }

    if (InPlayerName == "asdfasdf") {
        //#. 임의로 이름에 따라 색깔 지정
        PlayerInGameInfo.TeamColor = "Red";
    }

    PlayerInGameInfo.PlayerName = InPlayerName;
    PlayerInGameInfo.Kill = 0;
    PlayerInGameInfo.Death = 0;
    PlayerInGameInfo.Assist = 0;

    //#. 로그인 시에는 모두 그룹 0에 들어감
    //#. 그룹 0 : Lobby, 이후에는 모두 게임 방
    PlayerInGameInfo.Group = 0;

    bIsLoggedIn = true;
    UE_LOG(LogTemp, Log, TEXT("In PlayerState: %s"), bIsLoggedIn ? "T" : "F");
}

//////////////////////////////////////////////////////////////////////////////

void AK_YG_LobbyPlayerState::SetMasterVolume(float volume)
{
    MasterVolume = volume;
}

void AK_YG_LobbyPlayerState::SetEffectVolume(float volume)
{
    EffectVolume = volume;
}

void AK_YG_LobbyPlayerState::SetIsFullScreen(bool bIsFullScreenValue)
{
    bIsFullScreen = bIsFullScreenValue;
}

//////////////////////////////////////////////////////////////////////////////

float AK_YG_LobbyPlayerState::GetMasterVolume()
{
    return MasterVolume;
}

float AK_YG_LobbyPlayerState::GetEffectVolume()
{
    return EffectVolume;
}

bool AK_YG_LobbyPlayerState::GetIsFullScreen()
{
    return bIsFullScreen;
}