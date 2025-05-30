// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "OnlyFightAnimActorComponent.generated.h"

class UBaseCharacterAnimInstance;
enum class EIdleTurnType : uint8;

// Idle turn rotation variables
USTRUCT(BlueprintType)
struct FIdleRotate
{
	GENERATED_BODY()

	UPROPERTY()
	EIdleTurnType TurnDirection;

	UPROPERTY()
	float TargetYaw;

	// === Initializer ===
	FIdleRotate() {}
	FIdleRotate(EIdleTurnType InTurnDirection, float InTargetYaw) : TurnDirection(InTurnDirection), TargetYaw(InTargetYaw) {}
};

// Need to update every frame datas
USTRUCT(BlueprintType)
struct FTickUpdateData
{
	GENERATED_BODY()
	
	// Player input's direction
	UPROPERTY()
	FVector2D InputWASD;

	// Player input's aim pitch
	UPROPERTY()
	float InputAimPitch;

	// Player input's aim yaw
	UPROPERTY()
	float InputAimYaw;

	// === Initializer ===
	FTickUpdateData() {}
	FTickUpdateData(FVector2D InInputWASD, float InInputAimPitch, float InInputAimYaw) : InputWASD(InInputWASD), InputAimPitch(InInputAimPitch), InputAimYaw(InInputAimYaw){}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UOnlyFightAnimActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOnlyFightAnimActorComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// =================================
	// ======== RPC - To Server ========

	// To Server - Can sit interaction?
	UFUNCTION(Server, Reliable)
	void ServerCanSitInteraction();

	// To Server - New player's data
	UFUNCTION(Server, Unreliable)
	void ServerNewPlayerData(const FTickUpdateData InTickUpdateData);

	// To Server - New player's idle turn direction
	UFUNCTION(Server, Reliable)
	void ServerNewTurnDirection(const FIdleRotate InTurnInfo);

	// To Server - Set sit variables initialization
	UFUNCTION(Server, Reliable)
	void ServerInitializeSitVariables();

	// To Server - Set current character rotation value
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateCharacdterRotation(const FRotator InNewRotation);

	// To Server - Update player camera's rotation

	// =================================
	// ======== RPC - Multicast ========

	// Multicast - New player's idle turn direction
	UFUNCTION(NetMulticast, Reliable)
	void MulticastNewTurnDirection(const FIdleRotate InTurnInfo);

	// Multicast - New player's data with updated every frame
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastNewPlayerData(const FTickUpdateData InTickUpdateData);

	// =================================
	// ======= Client --> Server =======

	// Check is it possible to sit interaction?
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion")
	bool CheckSitInteraction();

	// Check is need to update player data?
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion")
	bool CheckUpdatePlayerData(const FTickUpdateData InTickUpdateData);

	// Check is need to update idle turn direction?
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion")
	bool CheckUpdateTurnDirection(const FIdleRotate& InTurnInfo);

	// Check is need to sitting initialization?
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion")
	bool CheckInitializeSitVariables();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Set replicated variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// =================================
	// ======== RPC - To Server ========

	// To Server - Execute to can change character's sitting state
	void ServerCanSitInteraction_Implementation();

	// To Server - Execute to can change player input direction value
	void ServerNewPlayerData_Implementation(const FTickUpdateData InTickUpdateData);

	// To Server - Execute to can change idle turn direction value
	void ServerNewTurnDirection_Implementation(const FIdleRotate InTurnInfo);

	// To Server - Execute to set sit variables initialization
	void ServerInitializeSitVariables_Implementation();

	// To Server - Execute to update current character rotation
	void ServerUpdateCharacdterRotation_Implementation(const FRotator InNewRotation);

	//// To Server - Examine to valid function which can change character's sitting state
	//bool ServerCanSitInteraction_Validate();

	// To Server - Examine to valid function which can update current character rotation
	bool ServerUpdateCharacdterRotation_Validate(const FRotator InNewRotation);

	// =================================
	// ======== RPC - Multicast ========

	// Multicast - New player's idle turn direction
	void MulticastNewTurnDirection_Implementation(const FIdleRotate InTurnInfo);

	// Multicast - New player's data with updated every frame
	void MulticastNewPlayerData_Implementation(const FTickUpdateData InTickUpdateData);

	// =================================
	// ============= OnRep =============

	// It is auto calling, when 'bIsSittingState' value change 
	UFUNCTION()
	void OnRep_IsSittingState();

	// It is auto calling, when 'PlayerInputWASD' value change
	UFUNCTION()
	void OnRep_PlayerInputWASD();

	// It is auto calling, when 'UpdateCharacterRotatation' value change
	UFUNCTION()
	void OnRep_UpdateCharacterRotatation();

private:

	// =================================
	// ====== Character Rotations ====== 
	
	// Calculate character rotate direction
	void CalculateRotateDirection(const float InDeltaTime);

	// Update character rotate direction
	void UpdateRotateDirection(const float InDeltaTime);

	// Check player character is possible to turn
	bool IsPlayerCharacterTurn();

	// Initialize calculated rotation variables
	UFUNCTION()
	void InitializeRotationVariables();

	// Update character head's rotation with aim offset
	void UpdateRotateAimOffset();

	// Set anim instance 
	void InitializeAnimInstance();

private:

	// Character's sitting state
	UPROPERTY(ReplicatedUsing = OnRep_IsSittingState, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	bool bIsSittingState;

	// Character's anim instance
	UPROPERTY(BlueprintReadOnly, Category = "K_YG|Animation", meta = (AllowPrivateAccess = true))
	class UBaseCharacterAnimInstance* AnimInstance;

	// Character's instance
	UPROPERTY(BlueprintReadOnly, Category = "K_YG|Animation", meta = (AllowPrivateAccess = true))
	class ACharacter* Character;

	// Player input direction
	UPROPERTY(ReplicatedUsing = OnRep_PlayerInputWASD, BlueprintReadOnly, Category = "K_YG|Locomotion", meta = (AllowPrivateAccess = true))
	FVector2D PlayerInputWASD;

	// Turn direction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", meta = (AllowPrivateAccess = true))
	FIdleRotate TurnDirection = FIdleRotate();

	// ===============================================
	// ===== Update Character Rotation Variables =====

	// Character need to turn when character state is idle and actor & control rotation is not equal
	bool bIsTurnRotate = false;

	// Start character rotation - This is character's get actor rotation
	FRotator StartRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Target character rotation - This is player's control rotation
	FRotator TargetRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Rotate rate
	float RotateRate = 0.0f;

	// Rotation progress
	float RotateProgress = 0.0f;

	// Player character's lower rotation diff by control rotation & actor rotation
	const float LowerRotateThreshold = 80.0f;

	// Player character's higher rotation diff by control rotation & actor rotation
	const float HigherRotateThreshold = 150.0f;

	// Prevent send rotation data multiple
	bool bIsSendedRotationData = false;

	// ==========================================================
	// ===== Calculate character rotate direction variables =====

	// When player instant rotate permission time
	const float InstantRotateThresholdTime = 0.5f;

	// Current player instant rotate time
	float CurrentInstantRotateTime = 0.0f;

	// Camera rotation which is player desired direction and character rotation difference value
	float CameraAndCharacterRotationDiff = 0.0f;

	// ==========================================================
	// ================ Player camera's rotation ================

	// Current player camera's rotation which is measured every tick
	FRotator CurrentCameraRotation = FRotator::ZeroRotator;

	// The last player camera's rotation data send to the server
	FRotator LastCameraRotation = FRotator::ZeroRotator;

	// 'CurrentCameraRotation' data sends to server When 'CurrentCameraRotation' & 'LasetCameraRotation' diff is over this threshold
	float ThresholdCameraRotation = 1.0f;

	// Update current character rotation variable
	UPROPERTY(ReplicatedUsing = OnRep_UpdateCharacterRotatation, BlueprintReadOnly, Category = "K_YG|Locomotion", meta = (AllowPrivateAccess = true))
	FRotator UpdateCharacterRotatation;
};
