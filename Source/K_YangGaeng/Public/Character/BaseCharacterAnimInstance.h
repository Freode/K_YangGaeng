// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

DECLARE_DELEGATE(FOnNoInputParam);
DECLARE_DELEGATE_OneParam(FOnBoolInputParam, const bool);
DECLARE_DELEGATE_RetVal(bool, FOnBoolOutputParam);

// Forward Declaration
enum class EWeapon : uint8;

// Turn direction
UENUM(BlueprintType)
enum class EIdleTurnType : uint8
{
	None,
	TurnLeft90,
	TurnRight90,
	TurnLeft180,
	TurnRight180
};

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UBaseCharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Current character variable is valid?
	UFUNCTION(BlueprintCallable, Category = "K_YG|Animation")
	bool IsValidCurrentCharacter();

	// Get Speed of Ground ( X & Y )
	UFUNCTION(BlueprintPure, Category = "K_YG|Locomotion")
	float GetGroundSpeed() { return MoveVelocity.Size2D(); }

	// Calculate acceleration and checking ground speed
	UFUNCTION(Category = "K_YG|Locomotion")
	bool CalculateShouldMove(ACharacter* Character);

	// Setter - InputWASD
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion")
	void SetInputWASD(const FVector2D NewInput){ InputWASD = NewInput; }

	// Return movement speed related current situation which is sit, run, etc.
	FVector2D ReturnMovementSpeedScaleValue(const FVector2D& InputValue);

	// Can Jump interaction?
	bool CanJumpInteraction();

	// ==================================
	// ============= Getter =============

	// Get character is sitting state?
	bool GetIsSittingState() const { return bIsSittingState; }

	// Get character is sitting interaction?
	bool GetIsSittingInteraction() const { return bIsSittingInteraction; }

	// Get character is moving?
	bool GetIsMoving() const { return bIsMoving; }

	// Get character is falling state?
	bool GetIsFalling() const { return bIsFalling; }

	// Get player's weapon obejct
	EWeapon GetAnimWeaponState() const { return AnimWeaponState; }

	// ==================================
	// ============= Setter =============

	// Set character is sitting state
	void SetIsSittingState(const bool bNewState) { bIsSittingState = bNewState; }

	// Set character's sitting interaction
	void SetIsSittingInteraction(const bool bNewState) { bIsSittingInteraction = bNewState; }

	// Set character turn idle direction
	void SetTurnDirection(const EIdleTurnType InTurnDirection) { TurnDirection = InTurnDirection; }

	// Set player's weapon object
	void SetAnimWeaponState(const EWeapon InAnimWeaponState) { AnimWeaponState = InAnimWeaponState; }

	// Set player's aim Yaw
	void SetPlayerAimYaw(const float InPlayerAimYaw) { PlayerAimYaw = InPlayerAimYaw; }

	// Set player's aim pitch
	void SetPlayerAimPitch(const float InPlayerAimPitch) { PlayerAimPitch = InPlayerAimPitch; }

	void SetIsPlayerAnimOn(const bool InbIsPlayerAnimOn) { bIsPlayerAimOn = InbIsPlayerAnimOn; }

	// Set player's reload pitch
	void SetIsPlayerReloadOn(const bool InbIspLayerAnimOn) { bIsPlayerReloadOn = InbIspLayerAnimOn; }

	// ==================================

	// Initialize all sit interaction's variable
	void InitializeSitting();

private:

	// ==================================
	// ========= Anim Montage ===========

	// Sit interaction start anim notify
	UFUNCTION()
	void AnimNotify_SitInteractionStart();

	// Sit interaction finish anim notify
	UFUNCTION()
	void AnimNotify_SitInteractionFinish();

	// Dash Skill interaction finish to play
	UFUNCTION()
	void AnimNotify_EndDashSkill();

	// Set start parkour cancel time
	UFUNCTION()
	void AnimNotify_StartParkourCancelTime();

	// Set end parkour cancel time
	UFUNCTION()
	void AnimNotify_EndParkourCancelTime();

	// Turn start when idle or sit idle state
	UFUNCTION()
	void AnimNotify_StartTurn();

	// Turn end when idle or sit idle state
	UFUNCTION()
	void AnimNotify_EndTurn();

	// Parkour interaction finish anim notify
	UFUNCTION()
	void AnimNotify_ParkourInteractionFinish();

	// ==================================
	// ============= ETC ================

	// Initialize character's default montage
	void InitializeDefaultMontage();

	// Lerp to blend upper body value
	void LerpToBlendUpperBodyValue(float DeltaSeconds);

	// Checking blend upper body value
	void CheckBlendUpperBodyValue();

public:

	// === Cofficient ===

	// Sit coefficient
	static constexpr float SIT_SPEED_COEFFICIENT = 0.65f;
	static constexpr float SITTING_SPEED_COEFFICIENT = 0.45f;

	// Blend Upper Body Value coefficient
	static constexpr float BLEND_UPPER_BODY_VALUE_COEFFICIENT = 0.8f;

	// === Delegate ===
	FOnNoInputParam OnEndDashSkillDelegate;
	FOnBoolInputParam OnParkourCancelTimeDelegate;
	FOnNoInputParam OnStartTurnDelegate;
	FOnNoInputParam OnParkourInteractionFinishDelegate;
	FOnBoolOutputParam OnSittingCancelDelegate;

private:

	// Player's input 'WASD' key value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", meta = (AllowPrivateAccess = true))
	FVector2D InputWASD;

	// Character's max velocity
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	FVector MaxMoveVelocity;

	// Character's current velocity
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	FVector MoveVelocity;

	// Charcter's current XY speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	float CurrentSpeedXY;

	// Character's sitting state
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	bool bIsSittingState;

	// Character wants to specific direction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	float Direction;

	// AnimInstance's owner pawn
	UPROPERTY(Transient, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	class ACharacter* CurrentCharacter;

	// Character's jumping state
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	bool bIsFalling;
	
	// Character's should move - not falling 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	bool bShouldMove;

	// Character's turn direction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	EIdleTurnType TurnDirection;
	
	// Is character moving state?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	bool bIsMoving;

	// Sit interaction operate?
	bool bIsSittingInteraction = false;

	// Player gets weapon object
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Weapon", Meta = (AllowPrivateAccess = true))
	EWeapon AnimWeaponState;

	// Player's aim rotation with pitch
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	float PlayerAimPitch;

	// Player's aim rotation with yaw
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	float PlayerAimYaw;

	// Ignore upper body animation when playing something anim instance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	float BlendUpperBodyValue = 1.0f;

	// When this variable is changed, modify BlendUpperBodyValue is modified every frame.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion", Meta = (AllowPrivateAccess = true))
	bool bIsBlendUpperBodyValueChanged;

	// Cach which is bIsBlendUpperBodyValueChanged;
	bool bIsPreviousBlendUpperBodyValueChanged;

	// Character is aim on because of player's input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Weapon", Meta = (AllowPrivateAccess = true))
	bool bIsPlayerAimOn;

	// Character's default montage
	UAnimMontage* DefaultMontage;

	// Character is reload beacuse of plyer's input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Weapon", Meta = (AllowPrivateAccess = true))
	bool bIsPlayerReloadOn;
	
};
