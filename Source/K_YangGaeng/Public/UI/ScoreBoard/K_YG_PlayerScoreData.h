// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "UObject/NoExportTypes.h"
#include "K_YG_PlayerScoreData.generated.h"



/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UK_YG_PlayerScoreData : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerScoreData", Meta = (AllowPrivateAccess = true))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerScoreData", Meta = (AllowPrivateAccess = true))
	FString Eliminations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerScoreData", Meta = (AllowPrivateAccess = true))
	FString Eliminated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerScoreData", Meta = (AllowPrivateAccess = true))
	FString Resists;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerScoreData", Meta = (AllowPrivateAccess = true))
	FString Ping;

private:

protected:
	
};
