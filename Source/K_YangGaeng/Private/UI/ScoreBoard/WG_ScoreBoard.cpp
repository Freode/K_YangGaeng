// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/ScoreBoard/WG_ScoreBoard.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "UI/ScoreBoard/WG_PlayerGameInfo.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "UI/ScoreBoard/WG_PlayerGameInfo.h"

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

void UWG_ScoreBoard::AddPlayerScore(FString PlayerName, FString Eliminations, FString Eliminated, FString Asists, FString Ping)
{
	int32 PlayerCount = 0;
	if (PlayerCount > 5) {
		K_YG_SIMPLE(Warning);
	}
}


