// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Character/OnlyFightAnimActorComponent.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UOnlyFightAnimActorComponent::UOnlyFightAnimActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set replicated possible
	SetIsReplicated(true);

	// Default player input wasd
	PlayerInputWASD = FVector2D(0.0f, 0.0f);

	// Set player charcter's instance
	Character = Cast<ACharacter>(GetOwner());

	// Initialization
	TurnDirection = FIdleRotate(EIdleTurnType::None, 0.0f);
}

// Check is it possible to sit interaction?
bool UOnlyFightAnimActorComponent::CheckSitInteraction()
{
	// When sit interaction is operating and character is falling,
	// extra input interactions are ignored as sit interaction finished.
	if (AnimInstance->GetIsSittingInteraction() || AnimInstance->GetIsFalling())
	{
		return false;
	}
	K_YG_SIMPLE(Error);
	ServerCanSitInteraction();
	return true;
}

// Check is need to update player input direction value?
bool UOnlyFightAnimActorComponent::CheckUpdatePlayerData(const FTickUpdateData InTickUpdateData)
{
	//// Not equal previous input direction
	//if (PlayerInputWASD.Equals(InTickUpdateData.InputWASD, KINDA_SMALL_NUMBER) == false )
	//{
	//	ServerNewPlayerInputWASD(InInputWASD);
	//	return true;
	//}
	ServerNewPlayerData(InTickUpdateData);

	return true;
}

// Check is need to update idle turn direction?
bool UOnlyFightAnimActorComponent::CheckUpdateTurnDirection(const FIdleRotate& InTurnInfo)
{
	// Not equal previous character idle turn direction
	if (TurnDirection.TurnDirection != InTurnInfo.TurnDirection)
	{
		ServerNewTurnDirection(InTurnInfo);
		return true;
	}

	return false;
}

// Check is need to sitting initialization?
bool UOnlyFightAnimActorComponent::CheckInitializeSitVariables()
{
	// Return if it isn't conrolled pawn
	if (Character->IsLocallyControlled() == false)
	{
		return false;
	}

	K_YG_SIMPLE(Warning);
	// Sitting variables initialization
	bIsSittingState = false;
	AnimInstance->InitializeSitting();
	ServerInitializeSitVariables();

	return true;
}

// Called when the game starts
void UOnlyFightAnimActorComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	K_YG_CHECK(Character != nullptr);

	InitializeAnimInstance();

	// Binding
	AnimInstance->OnStartTurnDelegate.BindUObject(this, &UOnlyFightAnimActorComponent::InitializeRotationVariables);
}

// Set replicated variables
void UOnlyFightAnimActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ===== Replicated Variable =====
	DOREPLIFETIME(UOnlyFightAnimActorComponent, bIsSittingState);
	DOREPLIFETIME(UOnlyFightAnimActorComponent, PlayerInputWASD);
	DOREPLIFETIME(UOnlyFightAnimActorComponent, UpdateCharacterRotatation);
}

// To Server - Execute to can change character's sitting state
void UOnlyFightAnimActorComponent::ServerCanSitInteraction_Implementation()
{
	bIsSittingState = !(bIsSittingState);
}

// To Server - Execute to can change player input data
void UOnlyFightAnimActorComponent::ServerNewPlayerData_Implementation(const FTickUpdateData InTickUpdateData)
{
	MulticastNewPlayerData(InTickUpdateData);
}

// To Server - Execute to can change idle turn direction value
void UOnlyFightAnimActorComponent::ServerNewTurnDirection_Implementation(const FIdleRotate InTurnInfo)
{
	// K_YG_UELOG(Warning, TEXT("Server - Direction : %d, target yaw : %f"), InTurnInfo.TurnDirection, InTurnInfo.TargetYaw);
	TurnDirection = InTurnInfo;
	MulticastNewTurnDirection(InTurnInfo);
}

// To Server - Execute to set sit variables initialization
void UOnlyFightAnimActorComponent::ServerInitializeSitVariables_Implementation()
{
	bIsSittingState = false;
	AnimInstance->InitializeSitting();
}

// To Server - Execute to update current character rotation
void UOnlyFightAnimActorComponent::ServerUpdateCharacdterRotation_Implementation(const FRotator InNewRotation)
{
	UpdateCharacterRotatation = InNewRotation;
}

// To Server - Examine to valid function which can update current character rotation
bool UOnlyFightAnimActorComponent::ServerUpdateCharacdterRotation_Validate(const FRotator InNewRotation)
{
	if (FMath::Abs(InNewRotation.Yaw - LastCameraRotation.Yaw) > ThresholdCameraRotation)
	{
		LastCameraRotation = InNewRotation;
		return true;
	}
	return false;
}

// Multicast - New player's idle turn direction
void UOnlyFightAnimActorComponent::MulticastNewTurnDirection_Implementation(const FIdleRotate InTurnInfo)
{
	if (InTurnInfo.TurnDirection != EIdleTurnType::None)
	{
		TurnDirection = InTurnInfo;
		AnimInstance->SetTurnDirection(TurnDirection.TurnDirection);
		RotateProgress = 0.0f;
		RotateRate = 1.0f;
		StartRotation = Character->GetActorRotation();
		TargetRotation = FRotator(0.0f, TurnDirection.TargetYaw, 0.0f);
		bIsTurnRotate = true;

		TurnDirection.TurnDirection = EIdleTurnType::None;
	}
}

// Multicast - New player's data with updated every frame
void UOnlyFightAnimActorComponent::MulticastNewPlayerData_Implementation(const FTickUpdateData InTickUpdateData)
{
	PlayerInputWASD = InTickUpdateData.InputWASD;
	
	InitializeAnimInstance();
	// Update each player's input WASD value
	AnimInstance->SetInputWASD(PlayerInputWASD);

	// K_YG_UELOG(Warning, TEXT("Input Aim Pitch : %f"), InTickUpdateData.InputAimPitch);
	//K_YG_UELOG(Warning, TEXT("%s - Input Aim Pitch : %f"), *K_YG_CLIENT_IDENTIFIER(), InTickUpdateData.InputAimPitch);
	// K_YG_PRINT_A_FRAME(FColor::Blue, TEXT("%s - Input Aim Pitch : %f"), *K_YG_CLIENT_IDENTIFIER(), InTickUpdateData.InputAimPitch);
	
	AnimInstance->SetPlayerAimYaw(InTickUpdateData.InputAimYaw);
	AnimInstance->SetPlayerAimPitch(InTickUpdateData.InputAimPitch);
}

// It is auto calling, when 'bIsSittingState' change value 
void UOnlyFightAnimActorComponent::OnRep_IsSittingState()
{
	// Set new sitting state to anim instance
	AnimInstance->SetIsSittingState(bIsSittingState);
}

// It is auto calling, when 'PlayerInputWASD' value change
void UOnlyFightAnimActorComponent::OnRep_PlayerInputWASD()
{
	AnimInstance->SetInputWASD(PlayerInputWASD);
}

// It is auto calling, when 'UpdateCharacterRotatation' value change
void UOnlyFightAnimActorComponent::OnRep_UpdateCharacterRotatation()
{
	FName UpperBodyBoneName = FName(TEXT("spine_01"));

	FTransform UpperBodyTransform = FTransform(UpdateCharacterRotatation);

	//Character->GetMesh()
	/*Character->GetMesh()->SetBoneRo*/
}

// Calculate character rotate direction
void UOnlyFightAnimActorComponent::CalculateRotateDirection(const float InDeltaTime)
{
	FIdleRotate NewTurnInfo;
	CameraAndCharacterRotationDiff = Character->GetControlRotation().Yaw - Character->GetActorRotation().Yaw;
	// When player is moving state, all charater rotate direction's variable is initialization
	if (AnimInstance->GetIsMoving())
	{
		NewTurnInfo.TurnDirection = EIdleTurnType::None;
		InitializeRotationVariables();
	}
	// When moving and turn rotate state are false
	// Measure rotate time when over the lower rotation threshold by character's contorl & actor rotation diff
	else if (IsPlayerCharacterTurn())
	{
		// Need to update rotation? 
		bool bNeedToUpdate = false;

		// Measure rotate time over the 
		CurrentInstantRotateTime = CurrentInstantRotateTime + InDeltaTime;

		// Modify value
		if (CameraAndCharacterRotationDiff > 180.0f) 
		{
			// Prevent over 540.0f
			CameraAndCharacterRotationDiff += 180.0f;
			int32 RoundNum = CameraAndCharacterRotationDiff / 360.0f;
			CameraAndCharacterRotationDiff -= (180.0f + 360.0f * RoundNum);
		}
		else if (CameraAndCharacterRotationDiff < -180.0f) 
		{ 
			// Prevent under -540.0f
			CameraAndCharacterRotationDiff -= 180.0f;
			int32 RoundNum = (-1) * CameraAndCharacterRotationDiff / 360.0f;
			CameraAndCharacterRotationDiff += (180.0f + 360.0f * RoundNum); 
		}

		//K_YG_UELOG(Warning, TEXT("Diff : %f"), CameraAndCharacterRotationDiff);

		// Instant roation changed check
		if (CurrentInstantRotateTime > InstantRotateThresholdTime && CurrentInstantRotateTime < InstantRotateThresholdTime + 0.25f)
		{
			if (CameraAndCharacterRotationDiff > HigherRotateThreshold)
			{
				NewTurnInfo.TurnDirection = EIdleTurnType::TurnRight180;
				bNeedToUpdate = true;
			}
			else if (CameraAndCharacterRotationDiff < (-1) * HigherRotateThreshold)
			{
				NewTurnInfo.TurnDirection = EIdleTurnType::TurnLeft180;
				bNeedToUpdate = true;
			}
			else if (CameraAndCharacterRotationDiff > LowerRotateThreshold)
			{
				NewTurnInfo.TurnDirection = EIdleTurnType::TurnRight90;
				bNeedToUpdate = true;
			}
			else if (CameraAndCharacterRotationDiff < (-1) * LowerRotateThreshold)
			{
				NewTurnInfo.TurnDirection = EIdleTurnType::TurnLeft90;
				bNeedToUpdate = true;
			}
			else
			{
				NewTurnInfo.TurnDirection = EIdleTurnType::None;
			}
		}

		// If player character needs to update rotation
		if (bNeedToUpdate && bIsSendedRotationData == false)
		{
			NewTurnInfo.TargetYaw = Character->GetActorRotation().Yaw + CameraAndCharacterRotationDiff;
			K_YG_UELOG(Warning, TEXT("Client - Direction : %d, target yaw : %f"), NewTurnInfo.TurnDirection, NewTurnInfo.TargetYaw);
			ServerNewTurnDirection(NewTurnInfo);
			bIsSendedRotationData = true;
		}
	}
	// Do not anything
	else
	{
		InitializeRotationVariables();
	}
}

// Update character rotate direction
void UOnlyFightAnimActorComponent::UpdateRotateDirection(const float InDeltaTime)
{
	// Character custom rotation is possible && Not current moving state && Custom rotation is progressing
	if (bIsTurnRotate && AnimInstance->GetIsMoving() == false && RotateProgress <= 1.0f)
	{
		// Add rotate progress
		RotateProgress = RotateProgress + InDeltaTime * RotateRate;

		// Character turn
		FRotator NewRotation = UKismetMathLibrary::RLerp(StartRotation, TargetRotation, RotateProgress, false);
		NewRotation.Roll = 0.0f;
		NewRotation.Pitch = 0.0f;
		Character->SetActorRotation(NewRotation);
	}
	// Initialize rotation setting
	else if (AnimInstance->GetIsMoving() || RotateProgress > 1.0f)
	{
		bIsTurnRotate = false;
		RotateProgress = 0.0f;
	}
}

// Check player character is possible to turn
bool UOnlyFightAnimActorComponent::IsPlayerCharacterTurn()
{
	// 1. Not moving ( Less than 1.0f velocity )
	// 2. Not current turning
	// 3. Bigger than lower rotate threshold
	// 4. Can moving state
	return AnimInstance->GetIsMoving() == false && bIsTurnRotate == false && FMath::Abs(CameraAndCharacterRotationDiff) > LowerRotateThreshold
		&& Character->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_None;
}

// Initialize calculated rotation variables
void UOnlyFightAnimActorComponent::InitializeRotationVariables()
{
	CurrentInstantRotateTime = 0.0f;
	CameraAndCharacterRotationDiff = 0.0f;
	bIsSendedRotationData = false;
}

// Update character head's rotation with aim offset
void UOnlyFightAnimActorComponent::UpdateRotateAimOffset()
{

}

// Set anim instance 
void UOnlyFightAnimActorComponent::InitializeAnimInstance()
{
	if (AnimInstance == nullptr)
	{
		// Get character's anim instance 
		AnimInstance = Cast<UBaseCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());
	}
}

// Called every frame
void UOnlyFightAnimActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character && Character->IsLocallyControlled())
	{
		// Player's rotation direction
		CalculateRotateDirection(DeltaTime);
	}

	// Update player's rotation by direction
	UpdateRotateDirection(DeltaTime);

	// Get player's current rotation
	CurrentCameraRotation = Character->GetControlRotation();

	// 
	if (Character->IsLocallyControlled() && CurrentCameraRotation.Yaw - LastCameraRotation.Yaw > ThresholdCameraRotation)
	{
		LastCameraRotation = CurrentCameraRotation;

	}
}

