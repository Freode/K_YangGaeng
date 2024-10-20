// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/ScoreBoard/WG_PlayerGameInfo.h"
#include "Components/TextBlock.h"

void UWG_PlayerGameInfo::NativeConstruct()
{
	Super::NativeConstruct();

	K_YG_SIMPLE(Warning);
}

void UWG_PlayerGameInfo::SetPlayerNameInWidget(const FString& PlayerName)
{
	K_YG_SIMPLE(Warning);
	TextBlock_PlayerName->SetText(FText::FromString(PlayerName));
}
