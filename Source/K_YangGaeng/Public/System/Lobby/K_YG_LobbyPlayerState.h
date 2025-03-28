// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "GameFramework/PlayerState.h"
#include "K_YG_LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YG_LobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    AK_YG_LobbyPlayerState();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
    bool bIsLoggedIn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI_Sound")
    float MasterVolume;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI_Sound")
    float EffectVolume;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI_Screen")
    bool bIsFullScreen;

    void Login(const FString& InPlayerName);

    ///////////////////////////////////////////////////////////////////////////////////////

    void SetMasterVolume(float volume);
    void SetEffectVolume(float volume);
    void SetIsFullScreen(bool bIsFullScreen);

    void SetPlayerTeamColor(FString TeamColor) {
        K_YG_SIMPLE(Warning);
        PlayerInGameInfo.TeamColor = TeamColor;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    float GetMasterVolume();
    float GetEffectVolume();
    bool GetIsFullScreen();

    FString GetPlayerTeamColor() {
        return PlayerInGameInfo.TeamColor;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    struct FPlayerInGameInfo
    {
        FString TeamColor;          //#. 캐릭터 팀 색깔
        FString PlayerName;         //#. 캐릭터 이름
        int16 Kill;                 //#. 캐릭터 킬 수
        int16 Death;                //#. 캐릭터 데스 수
        int16 Assist;               //#. 캐릭터 어시스트 수
        int16 Ping;                 //#. 캐릭터 현재 핑
        int16 Group;                //#. 캐릭터 그룹
    };
    
    FPlayerInGameInfo PlayerInGameInfo;
};
