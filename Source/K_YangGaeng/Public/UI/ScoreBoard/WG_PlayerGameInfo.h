// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "WG_PlayerGameInfo.generated.h"

class UTextBlock;
class UHorizontalBox;
class UK_YG_PlayerScoreData;
/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UWG_PlayerGameInfo : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Eliminations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Eliminated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Resists;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Ping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* HorizontalBox_BackGround;

	//#. player의 이름이 변경되지 않도록 const로 지정-
	UFUNCTION(BlueprintCallable, Category = "UI_ScoreBoard")
	void SetPlayerNameInWidget(const FString& PlayerName);

private:

protected:
	
};
