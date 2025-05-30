// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "WG_ScoreBoard.generated.h"

class AK_YG_LobbyPlayerState;
class AK_YG_LobbyPlayerController;
class UBorder;
class UListView;

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
	class UBorder* Border_TeamTotalScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border_EnemyTotalScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UListView* ListView_TeamList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UListView* ListView_EnemyList;

	class AK_YG_LobbyPlayerController* Mypc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerGameInfoClass;

	UFUNCTION(BlueprintCallable, Category = "UI_ScoreBoard")
	void ChangeBorderColor(FString TeamColor);

	UFUNCTION(BlueprintCallable, Category = "UI_ScoreBoard")
	void AddTeamPlayerScore(FString PlayerName, FString Eliminations, FString Eliminated, FString Resists, FString Ping);

	UFUNCTION(BlueprintCallable, Category = "UI_ScoreBoard")
	void AddEnemyPlayerScore(FString PlayerName, FString Eliminations, FString Eliminated, FString Resists, FString Ping);

private:
	
	int32 PlayerCounts = 0;

protected:

	//#. Widget에서도 키 입력을 받을 수 있도록 setting
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InputKeyEvent) override;
};
