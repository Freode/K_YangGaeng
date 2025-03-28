// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "GameFramework/GameStateBase.h"
#include "K_YG_LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YG_LobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AK_YG_LobbyGameState();

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	bool IsPlayerNamePresent(const FString& PlayerNameToCheck) const;

	UFUNCTION(BlueprintCallable, Category = "Player Management")
	void AddPlayerName(APlayerController* PlayerController, const FString& PlayerNameToCheck) const;

	//#. 모든 클라이언트에게 서버에서 변경되는 것을 알려줌
	UPROPERTY(Replicated)
	TArray<FString> PlayerNames;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
