// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/ScoreBoard/WG_ScoreBoard.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "UI/ScoreBoard/WG_PlayerGameInfo.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"

void UWG_ScoreBoard::NativeConstruct()
{
	Super::NativeConstruct();

	//#. 키바인딩을 위해 포커스를 true로 설정
	bIsFocusable = true;

	Mypc = Cast<AK_YG_LobbyPlayerController>(GetOwningPlayer());

	UWG_PlayerGameInfo* PlayerScoreList = CreateWidget<UWG_PlayerGameInfo>(this, UWG_PlayerGameInfo::StaticClass());

	//#. 생성은 되는데 진짜 빈거라서 아무것도 안보인다... 처음부터 다 만져줘야하는거같은데...
	//UTextBlock* TextBlock = NewObject<UTextBlock>(this);
	//TextBlock->SetText(FText::FromString("JAVA"));

	if (PlayerScoreList) {
		K_YG_SIMPLE(Warning);
		VerticalBox_TeamScoreBox->AddChildToVerticalBox(PlayerScoreList);
		PlayerScoreList->SetVisibility(ESlateVisibility::Visible);
		//#. 크러쉬가 난다..?
		//#. playername textblock을 못찾는중...왜?
		if (PlayerScoreList->TextBlock_PlayerName) {
			K_YG_SIMPLE(Warning);
			PlayerScoreList->TextBlock_PlayerName->SetText(FText::FromString("Hello"));
		}
	}
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

void UWG_ScoreBoard::AddPlayerListInScoreBoard()
{
	//#. 플레이어 리스트를 받아온다
	//#. vertical box에 접근한다
	//#. vertical box에 위젯을 추가한다
	//#. 추가하는 위젯이  WG_Playergameinfo 이다
	//#. 추가한 위젯에 각 text들에 접근한다.
	//#. 텍스트 들의 텍스트와 배경을 변경한다.
}
