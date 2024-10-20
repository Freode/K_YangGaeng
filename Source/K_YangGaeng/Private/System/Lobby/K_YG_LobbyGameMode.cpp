// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "System/Lobby/K_YG_LobbyGameMode.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyGameState.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "Character/K_YGOnlyFightCharacter.h"


AK_YG_LobbyGameMode::AK_YG_LobbyGameMode()
{
	//#. 리플리케이트 되도록 설정
	bReplicates = true;

	//#. LobbyPlayerController 연결
	PlayerControllerClass = AK_YG_LobbyPlayerController::StaticClass();

	//#. GameState class 연결
	GameStateClass = AK_YG_LobbyGameState::StaticClass();

	//#. PlayerState class 연결
	PlayerStateClass = AK_YG_LobbyPlayerState::StaticClass();
	
	//#. DefaultPawn 연결 => 캐릭터 생성
	DefaultPawnClass = AK_YGOnlyFightCharacter::StaticClass();
}

void AK_YG_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

