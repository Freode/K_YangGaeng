// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/ScoreBoard/WG_PlayerGameInfo.h"
#include "UI/ScoreBoard/K_YG_PlayerScoreData.h"
#include "Components/TextBlock.h"


void UWG_PlayerGameInfo::NativeConstruct()
{
	Super::NativeConstruct();
	//#. initialized
	K_YG_SIMPLE(Warning);
	TextBlock_PlayerName = nullptr;
	TextBlock_Eliminations = nullptr;
	TextBlock_Eliminated = nullptr;
	TextBlock_Resists = nullptr;
	TextBlock_Ping = nullptr;

}

void UWG_PlayerGameInfo::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	//#. listview는 모든 영역을 출력하지 않고 실제 보여질 영역의 데이터만 가져와서 보여줌
	//#. 출력 영역에 왔을 때 특정 이벤트를 통해 데이터 출력, 이때 그 특정 이벤트가 이 함수임
	//#. 변수 ListItemObject는 반드시 내가 만든 K_YG_PlayerScoreData 가 될 것임 (형 변환)
	UK_YG_PlayerScoreData* ScoreData = Cast<UK_YG_PlayerScoreData>(ListItemObject);
	//#. ??? header 파일에서 미리 ScoreData를 정의하고 빌드하면 빌드가 안된다..?

	//#. ScoreData에서 content 가져오기
	//#. 나의 경우 가져올 데이터가 5가지 존재(이름, 킬 수, 데스 수, 어시스트 수 , 핑)
	//#. 그냥 들고오면 안되고 Get으로 들고와야함
	if (ScoreData) {
		TextBlock_PlayerName->SetText(FText::FromString(ScoreData->PlayerName));
		TextBlock_Eliminations->SetText(FText::FromString(ScoreData->Eliminations));
		TextBlock_Eliminated->SetText(FText::FromString(ScoreData->Eliminated));
		TextBlock_Resists->SetText(FText::FromString(ScoreData->Resists));
		TextBlock_Ping->SetText(FText::FromString(ScoreData->Ping));
	}
	

	//#. widget이 업데이트 되도록 설정
	ForceLayoutPrepass();
}

void UWG_PlayerGameInfo::SetPlayerNameInWidget(const FString& PlayerName)
{
	K_YG_SIMPLE(Warning);
	TextBlock_PlayerName->SetText(FText::FromString(PlayerName));
}
