// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_ScoreBoard.generated.h"

class AK_YG_LobbyPlayerState;
class AK_YG_LobbyPlayerController;
class UVerticalBox;
class UBorder;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_ScoreBoard : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual bool IsFocusable() const {
		return true;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* VerticalBox_TeamScoreBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* VerticalBox_EnemyScoreBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border_TeamTotalScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border_EnemyTotalScore;

	class AK_YG_LobbyPlayerController* Mypc;

	UFUNCTION(BlueprintCallable, Category = "UI_ScoreBoard")
	void ChangeBorderColor(FString TeamColor);

	UFUNCTION(BlueprintCallable, Category = "UI_ScoreBoard")
	void AddPlayerListInScoreBoard();

private:
protected:

	//#. Widget에서도 키 입력을 받을 수 있도록 setting
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InputKeyEvent) override;
};
