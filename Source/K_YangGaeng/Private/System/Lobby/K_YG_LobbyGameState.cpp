// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "System/Lobby/K_YG_LobbyGameState.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "Net/UnrealNetwork.h"

AK_YG_LobbyGameState::AK_YG_LobbyGameState()
{
    //#. replicate 활성화
    bReplicates = true;
}

bool AK_YG_LobbyGameState::IsPlayerNamePresent(const FString& PlayerNameToCheck) const
{
    //#. array에 이름이 들어있는지 확인
    if (PlayerNames.Contains(PlayerNameToCheck)) {
        //#. 있다면 True(Login Failed)
        return true;
    }
    else
    {
        //#. 아니라면 False(Login Succeed)
        return false;
    }
    
}

//#. 인자로 이 함수를 요청한 playercontroller의 주소를 받고 이후에 이 안에서
//#. 그 주소를 가지고 있는 Playercontroller에게 결과값을 리턴하자
//#. return은 함수를 이용해보자 새로만들어서!
void AK_YG_LobbyGameState::AddPlayerName(APlayerController* PlayerController, const FString& PlayerNameToCheck) const
{
    //#. array에 이름이 들어있는지 확인
    if (!PlayerNames.Contains(PlayerNameToCheck))
    {
        //#. 위 조건 만족시 중복이 없다는 의미 임으로 Array에 이름 추가

        const_cast<TArray<FString>&>(PlayerNames).Add(PlayerNameToCheck);

        UE_LOG(LogTemp, Log, TEXT("Unique Names"));

        //#. Array에 있는 모든 요소 출력
        for (const FString& PlayerName : PlayerNames)
        {
            UE_LOG(LogTemp, Log, TEXT("Initialized Player Name: %s"), *PlayerName);
        }

        //#. 클라이언트 함수 실행
        Cast<AK_YG_LobbyPlayerController>(PlayerController)->ClientNamePassed(PlayerNameToCheck);
    }

    else {
        //#. 중복이 있다는 의미임으로 코드 실행 x
        UE_LOG(LogTemp, Log, TEXT("Not Unique Names"));

        //#. Array에 있는 모든 요소 출력
        for (const FString& PlayerName : PlayerNames)
        {
            UE_LOG(LogTemp, Log, TEXT("Initialized Player Name: %s"), *PlayerName);
        }

        Cast<AK_YG_LobbyPlayerController>(PlayerController)->ClientNameUnPassed(PlayerNameToCheck);
    }
}

void AK_YG_LobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //#. 다른 클라이언트 들에게 PlayerNames가 갱신 되었다는 것을 알림
    DOREPLIFETIME(AK_YG_LobbyGameState, PlayerNames);
}
