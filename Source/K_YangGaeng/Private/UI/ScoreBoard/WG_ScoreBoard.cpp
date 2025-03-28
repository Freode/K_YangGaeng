// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/ScoreBoard/WG_ScoreBoard.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "UI/ScoreBoard/WG_PlayerGameInfo.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "UI/ScoreBoard/WG_PlayerGameInfo.h"
#include "UI/ScoreBoard/K_YG_PlayerScoreData.h"

void UWG_ScoreBoard::NativeConstruct()
{
	Super::NativeConstruct();

	//#. 키바인딩을 위해 포커스를 true로 설정
	bIsFocusable = true;

	Mypc = Cast<AK_YG_LobbyPlayerController>(GetOwningPlayer());

	
}

//#. widget이 켜져있는 상태에서 종료하는 함수
FReply UWG_ScoreBoard::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InputKeyEvent)
{
	K_YG_SIMPLE(Warning);
	if (InputKeyEvent.GetKey() == EKeys::F)
	{
		K_YG_SIMPLE(Warning);
		RemoveFromParent();

		Mypc->SetInputMode(FInputModeGameOnly());
		Mypc->bShowMouseCursor = false;

		return FReply::Handled();

	}
	return Super::NativeOnKeyDown(MyGeometry, InputKeyEvent);
}

//#. 팀 색깔에 따른 Border색깔 변경
//#. 자신 팀의 색깔을 인자로 주면 색깔별로 적군 색깔까지 세팅
void UWG_ScoreBoard::ChangeBorderColor(FString TeamColor)
{
	K_YG_CHECK(Border_TeamTotalScore);
	K_YG_CHECK(Border_EnemyTotalScore);

	//#. border의 배경색 변수 가져오기
	FSlateBrush TeamBrush = Border_TeamTotalScore->Background;
	FSlateBrush EnemyBrush = Border_TeamTotalScore->Background;

	FLinearColor TeamNewColor;
	FLinearColor EnemyNewColor;
	
	if (TeamColor == "Red") {
		TeamNewColor = FLinearColor::Red;
		EnemyNewColor = FLinearColor::Blue;
	}

	else if (TeamColor == "Blue") {
		TeamNewColor = FLinearColor::Blue;
		EnemyNewColor = FLinearColor::Red;
	}
	else {
		K_YG_SIMPLE(Warning);
	}
	

	//#. 새로운 색상을 입히고 다시 배경색에 넣기
	TeamBrush.TintColor = FSlateColor(TeamNewColor);
	EnemyBrush.TintColor = FSlateColor(EnemyNewColor);
	Border_TeamTotalScore->SetBrush(TeamBrush);
	Border_EnemyTotalScore->SetBrush(EnemyBrush);
}

void UWG_ScoreBoard::AddTeamPlayerScore(FString PlayerName, FString Eliminations, FString Eliminated, FString Resists, FString Ping)
{
	if (PlayerCounts < 5) {
		K_YG_SIMPLE(Warning);
		PlayerCounts++;
	}
	else {
		//#. 플레이어가 맵에 모두 차있음 -> 추가적으로 들어오지 못하게 막아야함
		//#. 근데 플레이 이후에는 추가적으로 들어오지 못하는데 이걸 굳이 해야할까
	}

	//#. 혹시 몰라 한번더 cast
	//#. 채팅데이터 인스턴스를 생성하고 이를 input에서 받아온 정보를 토대로 initialize
	UK_YG_PlayerScoreData* PlayerScoreListInstance = NewObject<UK_YG_PlayerScoreData>(Cast<AK_YG_LobbyPlayerController>(Mypc));
	if (PlayerScoreListInstance) {
		PlayerScoreListInstance->PlayerName = PlayerName;
		PlayerScoreListInstance->Eliminations = Eliminations;
		PlayerScoreListInstance->Eliminated = Eliminated;
		PlayerScoreListInstance->Resists = Resists;
		PlayerScoreListInstance->Ping = Ping;
	}

	//#. 적팀의 경우 어떻게 해야할지 모르겠네...흐음
	ListView_TeamList->AddItem(PlayerScoreListInstance);
	//#. 스크롤 가장 밑으로 내려감 -> 현재는 필요없을듯
	//ListView_TeamList->ScrollToBottom();

}

void UWG_ScoreBoard::AddEnemyPlayerScore(FString PlayerName, FString Eliminations, FString Eliminated, FString Resists, FString Ping)
{
	if (PlayerCounts < 5) {
		K_YG_SIMPLE(Warning);
		PlayerCounts++;
	}

	//#. 혹시 몰라 한번더 cast
	//#. 채팅데이터 인스턴스를 생성하고 이를 input에서 받아온 정보를 토대로 initialize
	UK_YG_PlayerScoreData* PlayerScoreListInstance = NewObject<UK_YG_PlayerScoreData>(Cast<AK_YG_LobbyPlayerController>(Mypc));
	if (PlayerScoreListInstance) {
		PlayerScoreListInstance->PlayerName = PlayerName;
		PlayerScoreListInstance->Eliminations = Eliminations;
		PlayerScoreListInstance->Eliminated = Eliminated;
		PlayerScoreListInstance->Resists = Resists;
		PlayerScoreListInstance->Ping = Ping;
	}

	ListView_EnemyList->AddItem(PlayerScoreListInstance);

}


