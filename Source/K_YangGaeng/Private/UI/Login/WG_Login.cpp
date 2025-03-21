// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/Login/WG_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "System/Lobby/K_YG_LobbyGameMode.h"
#include "System/Lobby/K_YG_LobbyPlayerController.h"
#include "System/Lobby/K_YG_LobbyPlayerState.h"
#include "System/Lobby/K_YG_LobbyGameState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UWG_Login::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Duplicate)
	{
		K_YG_SIMPLE(Warning);
		Button_Duplicate->OnClicked.AddDynamic(this, &UWG_Login::OnNameCheckedButtonClicked);
	}

	if (Button_CreateChar) {
		Button_CreateChar->OnClicked.AddDynamic(this, &UWG_Login::OnCreateCharButtonClicked);
	}
}

//#. Duplicate button 클릭 시 아래 함수 실행
void UWG_Login::OnNameCheckedButtonClicked()
{
	//#. TextBox에 있는 String 가져오기
	FString InputNickName = UserNameTextBox->GetText().ToString();

	//#. player 이름의 길이가 이 범위 안에 들어있어야함
	if ((InputNickName.Len() < 12) && (InputNickName.Len() > 2))
	{
		//#. playercontroller , playerState 가져오기
		AK_YG_LobbyPlayerController* LocalPlayerController = Cast<AK_YG_LobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		//#. 서버에 이름 전달
		LocalPlayerController->ServerNameChecked(InputNickName);

		//#. 로그인 관련 내용은 playercontroller에 존재
	}
	else
	{
		//#. 이름 길이가 너무 길거나 짧다고 이야기
		StatusTextBlock->SetText(FText::FromString("Name is Too Long or Short!"));
	}

	
}

//#. CreateChar button 클릭 시 아래 함수 실행
void UWG_Login::OnCreateCharButtonClicked()
{
	AK_YG_LobbyPlayerController* LocalPlayerController = Cast<AK_YG_LobbyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	LocalPlayerController->Client_SwitchToMain();
}


void UWG_Login::DuplicationPassed()
{
	Button_Duplicate->SetIsEnabled(false);
	Button_CreateChar->SetIsEnabled(true);
}

void UWG_Login::DuplicationUnPassed()
{
	Button_Duplicate->SetIsEnabled(true);
	Button_CreateChar->SetIsEnabled(false);
}

void UWG_Login::SetTitleSucceed()
{
	StatusTextBlock->SetText(FText::FromString("Login Succeed!"));
	UserNameTextBox->SetIsEnabled(false);
}

void UWG_Login::SetTitleFailed()
{
	StatusTextBlock->SetText(FText::FromString("Login Faild!"));
}

