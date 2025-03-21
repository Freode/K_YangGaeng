// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "AIController.h"
#include "K_YGAIController.generated.h"

// Forward Declaration
class UBlackboardData;
class UBehaviorTree;
class AAIMoveCenterLocation;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	
	// Initiailization
	AK_YGAIController();

	// Possess
	virtual void OnPossess(APawn* InPawn) override;

	// ==============================
	// =========== Getter ===========

	const AAIMoveCenterLocation* GetPreviousTargetLocation() { return PreviousTargetLocation; }

	// ==============================
	// =========== Setter ===========

	void SetPreviousTargetLocation(AAIMoveCenterLocation* InCurrentTargetLocation) { PreviousTargetLocation = InCurrentTargetLocation; }

public:

	// Blackboard key datas
	static const FName MoveTargetActorPos;
	static const FName MoveTargetLocationPos;
	static const FName ShootingTargetPos;
	static const FName RotateToTargetPos;
	static const FName bIsShootingToTargetPos;
	static const FName MaxShootErrorRangePos;
	static const FName CurrentShootErrorRangePos;
	static const FName OpponentBulletFlyDirectionPos;


private:

	// Behavior tree asset
	UPROPERTY()
	class UBehaviorTree* BTAsset;
	
	// black board asset
	UPROPERTY()
	class UBlackboardData* BBAsset;

	// Previous target location which is ai character previous located
	UPROPERTY()
	class AAIMoveCenterLocation* PreviousTargetLocation;
	
};
