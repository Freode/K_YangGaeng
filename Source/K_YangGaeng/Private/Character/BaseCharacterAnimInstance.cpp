// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Character/BaseCharacterAnimInstance.h"
#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BaseWeapon.h"

UBaseCharacterAnimInstance::UBaseCharacterAnimInstance()
{
	K_YG_UELOG(Warning, TEXT("Begin : %s"), *GetName());

	// None weapon state
	AnimWeaponState = EWeapon::NONE;

	// Variable Initialization
	bIsBlendUpperBodyValueChanged = true;
	bIsPreviousBlendUpperBodyValueChanged = bIsBlendUpperBodyValueChanged;
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	InitializeDefaultMontage();

	// === Pawn's speed ===
	K_YG_SIMPLE_CHECK(IsValidCurrentCharacter());

	// X : Forward
	// Y : Right
	// Z : Jump

	// North : Y = -1, East : X = 1
	// Get character's velocity and ground speed
	FVector CharacterVelocity = CurrentCharacter->GetCharacterMovement()->Velocity;

	// Get current speed with X & Y
	CurrentSpeedXY = FVector2D(CharacterVelocity.X, CharacterVelocity.Y).Size();

	// InputWASD - X : Right, Y : Forward
	MoveVelocity = FVector(CurrentSpeedXY * InputWASD.Y, CurrentSpeedXY * InputWASD.X, CharacterVelocity.Z);

	// Falling 
	bIsFalling = CurrentCharacter->GetCharacterMovement()->IsFalling();

	// Ignored upperbody animation when player character's unarmed state
	// BlendUpperBodyValue = MoveVelocity.Size() <= 0.5f && AnimWeaponState == EWeapon::NONE ? 1.0f : 0.0f;
	
	// When character is sitting state and go to fall, sit interaction is canceled
	// It is operated only locally client
	if (bIsSittingState && bIsFalling && OnSittingCancelDelegate.IsBound() && CurrentCharacter->IsLocallyControlled())
	{
		OnSittingCancelDelegate.Execute();
		InitializeSitting();
	}

	// Decide character is moving state
	if (bIsMoving != CurrentSpeedXY > 1.0f)
	{
		bIsMoving = CurrentSpeedXY > 1.0f;
		CurrentCharacter->bUseControllerRotationYaw = bIsMoving;
	}

	// Should Move
	bShouldMove = CalculateShouldMove(CurrentCharacter);

	// Checking the upper body is changed. 
	CheckBlendUpperBodyValue();

	// When Blend Upper Body Value needs to change, it modify value with lerp 
	LerpToBlendUpperBodyValue(DeltaSeconds);


	// is reloading true
	bIsPlayerReloadOn = false;

}

// Current character variable is valid ?
bool UBaseCharacterAnimInstance::IsValidCurrentCharacter()
{
	if (CurrentCharacter == nullptr)
	{
		CurrentCharacter = Cast<ACharacter>(TryGetPawnOwner());
		return false;
	}
	return true;
}

// Calculate acceleration and checking ground speed
bool UBaseCharacterAnimInstance::CalculateShouldMove(ACharacter* Character)
{
	// Get aceleration
	FVector Acceleration = Character->GetCharacterMovement()->GetCurrentAcceleration();

	// Ground speed's upper than specific threshold and acceleration(input) is valid
	return (GetGroundSpeed() > 3.0f) && !Acceleration.Equals(FVector(0.0f, 0.0f, 0.0f), KINDA_SMALL_NUMBER);
}

// Return movement speed related current situation which is sit, run, etc.
FVector2D UBaseCharacterAnimInstance::ReturnMovementSpeedScaleValue(const FVector2D& InputValue)
{
	FVector2D NewInputValue = InputValue;

	// Modify movement rate - because of Forward ( Y = -1 , X = 0 ), Right&Forward ( Y = -1, X = 1 )
	// Diagonal movement is 40% faster than stragith movement.
	float TotalSize = FMath::Sqrt(NewInputValue.X * NewInputValue.X + NewInputValue.Y * NewInputValue.Y);
	if (TotalSize >= KINDA_SMALL_NUMBER)
	{
		NewInputValue = NewInputValue * (1 / TotalSize);
	}

	// When sit interaction is operating, character move slowly
	if (bIsSittingInteraction)
	{
		// K_YG_SIMPLE(Error);
		NewInputValue = NewInputValue * SITTING_SPEED_COEFFICIENT;
	}

	// When sit movement is slower than idle state
	if (bIsSittingState)
	{
		NewInputValue = NewInputValue * SIT_SPEED_COEFFICIENT;
	}

	return NewInputValue;
}

// Can Jump interaction?
bool UBaseCharacterAnimInstance::CanJumpInteraction()
{
	// When character's sit, jump interaction can't operate
	return (!bIsSittingState) && (!bIsSittingInteraction);
}

// Initialize all sit interaction's variable
void UBaseCharacterAnimInstance::InitializeSitting()
{
	bIsSittingState = false;
	bIsSittingInteraction = false;
}

// Sit interaction start anim notify
void UBaseCharacterAnimInstance::AnimNotify_SitInteractionStart()
{
	bIsSittingInteraction = true;
}

// Sit interaction finish anim notify
void UBaseCharacterAnimInstance::AnimNotify_SitInteractionFinish()
{
	bIsSittingInteraction = false;
}

// Dash Skill interaction finish to play
void UBaseCharacterAnimInstance::AnimNotify_EndDashSkill()
{
	K_YG_SIMPLE(Warning);
	InitializeSitting();
	if (OnEndDashSkillDelegate.IsBound())
	{
		OnEndDashSkillDelegate.Execute();
	}
}

// Set start parkour cancel time
void UBaseCharacterAnimInstance::AnimNotify_StartParkourCancelTime()
{
	if (OnParkourCancelTimeDelegate.IsBound())
	{
		OnParkourCancelTimeDelegate.Execute(true);
	}
}

// Set end parkour cancel time
void UBaseCharacterAnimInstance::AnimNotify_EndParkourCancelTime()
{
	if (OnParkourCancelTimeDelegate.IsBound())
	{
		OnParkourCancelTimeDelegate.Execute(false);
	}
}

// Turn start when idle or sit idle state
void UBaseCharacterAnimInstance::AnimNotify_StartTurn()
{
	TurnDirection = EIdleTurnType::None;

	if (OnStartTurnDelegate.IsBound())
	{
		OnStartTurnDelegate.Execute();
	}
}

// Turn end when idle or sit idle state
void UBaseCharacterAnimInstance::AnimNotify_EndTurn()
{
	// Modify?
	CurrentCharacter->bUseControllerRotationYaw = false;
}

// Parkour interaction finish anim notify
void UBaseCharacterAnimInstance::AnimNotify_ParkourInteractionFinish()
{
	if (OnParkourInteractionFinishDelegate.IsBound())
	{
		OnParkourInteractionFinishDelegate.Execute();
	}
}

// Initialize character's default montage
void UBaseCharacterAnimInstance::InitializeDefaultMontage()
{
	if (DefaultMontage == nullptr)
	{
		DefaultMontage = GetCurrentActiveMontage();
	}
}

// Lerp to blend upper body value
void UBaseCharacterAnimInstance::LerpToBlendUpperBodyValue(float DeltaSeconds)
{
	BlendUpperBodyValue += (bIsBlendUpperBodyValueChanged ? 1 : -1) * BLEND_UPPER_BODY_VALUE_COEFFICIENT * DeltaSeconds;
	BlendUpperBodyValue = FMath::Clamp(BlendUpperBodyValue, 0.0f, 1.0f);
}

// Checking blend upper body value
void UBaseCharacterAnimInstance::CheckBlendUpperBodyValue()
{
	// Not have any weapon
	if (AnimWeaponState == EWeapon::NONE)
	{
		bIsBlendUpperBodyValueChanged = !(bIsMoving || bIsFalling || DefaultMontage != GetCurrentActiveMontage());
	}
	// Have any weapon
	else
	{
		bIsBlendUpperBodyValueChanged = !(bIsFalling || DefaultMontage != GetCurrentActiveMontage());
	}
}
