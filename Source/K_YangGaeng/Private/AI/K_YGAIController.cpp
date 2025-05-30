// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "AI/K_YGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

// Blackboard key datas
const FName AK_YGAIController::MoveTargetActorPos(TEXT("MoveTargetActor"));
const FName AK_YGAIController::MoveTargetLocationPos(TEXT("MoveTargetLocation"));
const FName AK_YGAIController::ShootingTargetPos(TEXT("ShootingTarget"));
const FName AK_YGAIController::RotateToTargetPos(TEXT("RotateToTarget"));
const FName AK_YGAIController::bIsShootingToTargetPos(TEXT("bIsShootingToTarget"));
const FName AK_YGAIController::MaxShootErrorRangePos(TEXT("MaxShootErrorRange"));
const FName AK_YGAIController::CurrentShootErrorRangePos(TEXT("CurrentShootErrorRange"));
const FName AK_YGAIController::OpponentBulletFlyDirectionPos(TEXT("OpponentBulletFlyDirection"));


AK_YGAIController::AK_YGAIController()
{
	// Blackboard asset
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBOBJECT(TEXT("/Game/K_YangGaeng/AI/BB_K_YG_Character.BB_K_YG_Character"));
	if (BBOBJECT.Succeeded())
	{
		BBAsset = BBOBJECT.Object;
	}

	// Behaiovr Tree asset
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTOBJECT(TEXT("/Game/K_YangGaeng/AI/BT_K_YG_Character.BT_K_YG_Character"));
	if (BTOBJECT.Succeeded())
	{
		BTAsset = BTOBJECT.Object;
	}
}

// Possess
void AK_YGAIController::OnPossess(APawn* InPawn)
{

}
