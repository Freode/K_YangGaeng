// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "ParkourActorComponent.generated.h"

// Forward declaration
class UAnimMontage;
class UCurveVector;
class UCurveFloat;
namespace EDrawDebugTrace
{
	enum Type;
}

// Get player character's moving data delegate
DECLARE_DELEGATE_RetVal(const FVector2D, FOnInputCharacterMovingData);

// Set character moveable delegate
DECLARE_DELEGATE_OneParam(FOnSetCharacterMovableData, bool);

// Parkour type is distributed by jump length
UENUM(BlueprintType)
enum class EParkourType : uint8
{
	NONE,
	HIGH_PARKOUR,
	LOW_PARKOUR
};

// Structer for parkour asset information
USTRUCT(BlueprintType)
struct FParkourAssetSetting
{
	GENERATED_BODY()

	// Play parkour animation asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	class UAnimMontage* AnimMontage;

	// Character's transform is modified with this curve data when character plays parkour animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	class UCurveVector* PositionAndCorrectionCurve;

	// The modification value of starting parkour animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	FVector StartingOffset;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	float LowHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	float LowPlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	float LowStartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	float HighHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	float HighPlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Asset")
	float HighStartPosition;

	// === Initializer ===
	FParkourAssetSetting() : AnimMontage(nullptr), PositionAndCorrectionCurve(nullptr), StartingOffset(FVector::ZeroVector), LowHeight(0.0f), LowPlayRate(0.0f), LowStartPosition(0.0f), HighHeight(0.0f), HighPlayRate(0.0f), HighStartPosition(0.0f) {}

	FParkourAssetSetting(class UAnimMontage* InAnimMontage, class UCurveVector* InPositionAndCorrectionCurve, FVector InStartingOffset, float InLowHeight, float InLowPlayRate, float InLowStartPosition, float InHighHeight, float InHighPlayRate, float InHighStartPosition) : AnimMontage(InAnimMontage), PositionAndCorrectionCurve(InPositionAndCorrectionCurve), StartingOffset(InStartingOffset), LowHeight(InLowHeight), LowPlayRate(InLowPlayRate), LowStartPosition(InLowStartPosition), HighHeight(InHighHeight), HighPlayRate(InHighPlayRate), HighStartPosition(InHighStartPosition) {}
};

//
USTRUCT(BlueprintType)
struct FParkourTraceSetting
{
	GENERATED_BODY()

	// Check parkour interaction possibility within max height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Trace")
	float MaxLedgeHeight;

	// Check parkour interaction possibility within min height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Trace")
	float MinLedgeHeight;

	// Check parkour interaction possibility - Is character locates in front of ledge?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Trace")
	float ForwardReachDistance;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Trace")
	float ForwardTraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Trace")
	float DownwardTraceRadius;

	FParkourTraceSetting() : MaxLedgeHeight(0.0f), MinLedgeHeight(0.0f), ForwardReachDistance(0.0f), ForwardTraceRadius(0.0f), DownwardTraceRadius(0.0f) {}

	FParkourTraceSetting(float InMaxLedgeHeight, float InMinLedgeHeight, float InForwardReachDistance, float InForwardTraceRadius, float InDownwardTraceRadius) : MaxLedgeHeight(InMaxLedgeHeight), MinLedgeHeight(InMinLedgeHeight), ForwardReachDistance(InForwardReachDistance), ForwardTraceRadius(InForwardTraceRadius), DownwardTraceRadius(InDownwardTraceRadius) {}
};

//
USTRUCT(BlueprintType)
struct FParkourParams
{
	GENERATED_BODY()

	// Play parkour animation asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Params")
	class UAnimMontage* AnimMontage;

	// Character's transform is modified with this curve data when character plays parkour animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Params")
	class UCurveVector* PositionAndCorrectionCurve;

	// The modification value of starting parkour animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Params")
	FVector StartingOffset;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Params")
	float StartingPosition;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "K_YG|Locomotion|Parkour|Params")
	float PlayRate;

	// === Initializer === 
	FParkourParams() : AnimMontage(nullptr), PositionAndCorrectionCurve(nullptr), StartingOffset(FVector::ZeroVector), StartingPosition(0.0f), PlayRate(0.0f) {}

	FParkourParams(class UAnimMontage* InAnimMontage, class UCurveVector* InPositionAndCorrectionCurve, FVector InStartingOffset, float InStartingPosition, float InPlayRate) : AnimMontage(InAnimMontage), PositionAndCorrectionCurve(InPositionAndCorrectionCurve), StartingOffset(InStartingOffset), StartingPosition(InStartingPosition), PlayRate(InPlayRate) {}

};

// Use to send parkour animation data to all clients
USTRUCT(BlueprintType)
struct FParkourAnimData
{
	GENERATED_BODY()

	// 
	UPROPERTY()
	EParkourType ParkourType;

	//
	UPROPERTY()
	float StartingPosition;

	//
	UPROPERTY()
	float PlayRate;

	// === Initializer === 
	FParkourAnimData() : ParkourType(EParkourType::NONE), StartingPosition(0.0f), PlayRate(0.0f) {}

	FParkourAnimData(EParkourType InParkourType, float InStartingPosition, float PlayRate) : ParkourType(InParkourType), StartingPosition(InStartingPosition), PlayRate(PlayRate) {}
};

// Use to send parkour data to server
USTRUCT(BlueprintType)
struct FParkourSendData
{
	GENERATED_BODY()

	// 
	UPROPERTY()
	FParkourAnimData ParkourAnimData;

	//
	UPROPERTY()
	FVector StartingOffset;

	//
	UPROPERTY()
	FTransform ParkourTarget;

	//
	UPROPERTY()
	FTransform ParkourAnimationStartOffset;

	//
	UPROPERTY()
	FTransform ParkourActualStartOffset;

	// === Initializer === 
	FParkourSendData() : ParkourAnimData(FParkourAnimData()), StartingOffset(FVector::ZeroVector), ParkourTarget(FTransform::Identity), ParkourAnimationStartOffset(FTransform::Identity), ParkourActualStartOffset(FTransform::Identity) {}

	FParkourSendData(FParkourAnimData InParkourAnimData, FVector InStartingOffset, FTransform InParkourTarget, FTransform InParkourAnimationStartOffset, FTransform InParkourActualStartOffset) : ParkourAnimData(InParkourAnimData), StartingOffset(InStartingOffset), ParkourTarget(InParkourTarget), ParkourAnimationStartOffset(InParkourAnimationStartOffset), ParkourActualStartOffset(InParkourActualStartOffset) {}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class K_YANGGAENG_API UParkourActorComponent : public UActorComponent
{
	GENERATED_BODY()

	public:
	// Sets default values for this component's properties
	UParkourActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Check player character which is attached this component isn't equal nullptr value
	void CheckPlayerCharacter();

	// Get capusle's current world location with Z correction
	FVector GetCapsuleBaseLocation(float ZOffset);

	// Get current character's movement which is equal to player movement input
	FVector GetPlayerMovementInput();

	// Get capsule location from base
	FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset);

	// Get is playing parkour?
	bool GetIsPlayingParkour() const { return bIsPlayingParkour; }

	// Get is possible to cancel parkour interaction?
	bool GetIsPossibleToCancelParkour() const { return bIsPossibleCancelParkour; }

	// Set parkour interaction cancel state
	UFUNCTION()
	void SetParkourCanCancelTime(const bool bCanCancel);

	// Set parkour interacton & animation are finished.
	UFUNCTION()
	void ParkourIsFinished();

	/**
	* Parkour possiblility checked
	*
	* @return Can player character do parkour interaction?
	* @param OutParkourHeight - Player's character climbed that height
	* @param OutParkourWallTransform - Player's character climbed that total transform
	* @param OutParkourWallComponent - Wall which is climbed possible
	* @param OutParkourType - Play different parkour animation from climbed height
	*/
	bool CheckParkourPossible(float& OutParkourHeight, FTransform& OutParkourTarget, UPrimitiveComponent*& OutParkourWallComponent, EParkourType& OutParkourType);

	/**
	* Parkour animation play
	*
	*  @return Is parkour animation played normally?
	*  @param InParkourHeight - Player character climbs height by going target position
	*  @param InParkourTarget - Player character climbs to target position
	*  @param InParkourWallComponent - Player character climbs the target component ( wall or ledge )
	*  @param InParkourType - Player character plays parkour animation by climbed height
	*/
	bool StartParkour(const float& InParkourHeight, const FTransform& InParkourTarget, UPrimitiveComponent*& InParkourWallComponent, const EParkourType& InParkourType);

	/**
	*	Parkour animation stop
	*
	*	@return Is parkour animation stop successfully?
	*/
	bool StopParkour();

	/**
	*	Parkour animation reverse play
	*
	*	@return Is parkour animation stop successfully?
	*/
	bool ReverseParkour();

	// =================================
	// ======== RPC - To Server ========

	//
	UFUNCTION(Server, Reliable)
	void ServerParkourTransformTimeline(const FParkourSendData& InParkourSendData);

	// =================================
	// ======== RPC - Multicast ========

	//
	UFUNCTION(NetMulticast, Reliable)
	void MulticastParkourAnimationTimeline(const FParkourAnimData& InParkourAnimData);

protected:

	// =================================
	// ======== RPC - To Server ========

	// 
	void ServerParkourTransformTimeline_Implementation(const FParkourSendData& InParkourSendData);

	// =================================
	// ======== RPC - Multicast ========

	//
	void MulticastParkourAnimationTimeline_Implementation(const FParkourAnimData& InParkourAnimData);

private:

	/**
	* Use capsule collision to check wall is in front of character. If it is valid, return wall and capsule collision's interaction location & interaction direction
	*
	* @return Is specific wall in front of player character?
	* @param InTraceSetting - Wall trace base setting values
	* @param OutInitialTraceImpactPoint - Wall and capsule collision's interaction location
	* @param OutInitialTraceNormal - Wall and capsule collision's interaction direction which is indirect collision to wall
	*/
	bool CheckWallIsFrontOfCharacter(const FParkourTraceSetting& InTraceSetting, FVector& OutInitialTraceImpactPoint, FVector& OutInitialTraceNormal);

	/**
	* Use capsule collision to check the wall height whether player's character can climb or not
	*
	* @return Is the wall's height possible to climb ?
	* @param InTraceSetting - Wall trace base setting values
	* @param InInitialTraceImpactPoint - Wall and capsule collision's interaction location
	* @param InInitialTraceNormal - Wall and capsule collision's interaction direction which is indirect collision to wall
	* @param OutDownTraceLocation - Wall's height which is player's character climbed over the wall location
	* @param OutHitComponent - Wall which is climbed possible
	*/
	bool CheckPlayerIsClimbedWallHeight(const FParkourTraceSetting InTraceSetting, const FVector InInitialTraceImpactPoint, const FVector InInitialTraceNormal, FVector& OutDownTraceLocation, UPrimitiveComponent*& OutHitComponent);

	/**
	* Use capsule collision to check there is noting on the wall to prevent player character fixed state
	*
	* @return Is there noting on the wall?
	* @param InInitialTraceNormal - Wall and capsule collision's interaction direction which is indirect collision to wall
	* @param InDownTraceLocation - Wall's height which is player's character climbed on the wall location
	* @param OutTargetTransform - Player's character climbed that total transform
	* @param OutClimbedHeight - Player's character climbed that height
	*/
	bool CheckNotingOverTheWall(const FVector InInitialTraceNormal, const FVector InDownTraceLocation, FTransform& OutTargetTransform, float& OutClimbedHeight);

	/**
	* Use line trace by single channel to check height to deep. Prevent to operate low parkour type which under height is large to fall for player character
	* 
	* @return Falling height which is add to climbed height and decide more exactly parkour type
	* @param InExamineLength - Examine line trace length
	*/
	float CheckUnderHeightWhenCharacterFalling(const float InExamineLength);

	// Starting position and play rate change new range based on climbed height 
	FParkourParams CreateParkourParams(const EParkourType& InParkourType, const float& InClimbedHeight);

	// Get parkour asset setting by parkour type
	FParkourAssetSetting GetParkourAssetSetting(const EParkourType& InParkourType);

	// Parkour wall change transform from world to local
	FTransform ChangeParkourWallTransformWorldToLocal(class UPrimitiveComponent* &InComponent, FTransform& InTransform);

	// Return actual start parkour offset
	FTransform GetActualStartParkourOffset();

	// Return calculate parkour animation start offset
	FTransform CalculateParkourAnimationStartOffset();

	// Start Parkour Step 6. Play parkour timeline
	void PlayParkourTimeline();

	// Start Parkour Step 7. Play parkour animation
	bool PlayParkourAnimation();

	// ========== Parkour Timeline ==============

	// Execute every one frame(tick) when parkour timeline is progressing(update)
	UFUNCTION()
	void ParkourTimelineUpdate(float Value);

	// Execute when parkour timeline progress finish
	UFUNCTION()
	void ParkourTimelineEnd();

	// ========================================== 

	// ======== Parkour Timeline Update =========

	// Update Parkour Character Step 2. Get each curve's alpha data with parkour timeline progress position
	void GetParkourTimelineCurvesAlpha(float& OutPositionAlpha, float& OutXYCorrectionAlpha, float& OutZCorrectionAlpha);

	// Update Parkour Character Step 3. Lerp player's character transform which active parkour interaction
	void GetLerpedCurrentPlayerTransform(const float& InPositionAlpha, const float& InXYCorrectionAlpha, const float& InZCorrectionAlpha, const float& InProgressAlpha, FTransform& OutLerpedTarget);

	// ==========================================

public:

	// Get Player's mouse moving data delegate
	FOnInputCharacterMovingData OnInputCharacterMovingData;

	// Get Player character's movable data delegate
	FOnSetCharacterMovableData OnSetCharacterMovableData;

private:

	// Player character object which is attached this parkour component
	UPROPERTY(BlueprintReadOnly, Category = "K_YG|Locomotion|Parkour", meta = (AllowPrivateAccess = true))
	class ACharacter* Character;

	// When climb high height wall, high parkour animation is played
	UPROPERTY()
	class UAnimMontage* HighParkourAnimation;

	// When climb low height wall, low parkour animation is played
	UPROPERTY()
	class UAnimMontage* LowParkourAnimation;

	// When climb high height wall, player character's is relocated by high parkour curve vector
	UPROPERTY()
	class UCurveVector* HighParkourCurveVector;

	// When climb low height wall, player character's is relocated by low parkour curve vector
	UPROPERTY()
	class UCurveVector* LowParkourCurveVector;

	// Parkour play time curve data
	UPROPERTY()
	class UCurveFloat* ParkourTimlineCurveData;

	// Parkour timeline component
	UPROPERTY()
	UTimelineComponent* ParkourTimeline;

	// Constant - Debug draw time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion|Parkour", meta = (AllowPrivateAccess = true))
	float PARKOUR_DEBUG_DRAW_TIME;

	// Constant - Debug Type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion|Parkour", meta = (AllowPrivateAccess = true))
	TEnumAsByte<EDrawDebugTrace::Type> PARKOUR_DEBUG_TYPE;

	// Parkour timeline's delegate object
	FOnTimelineFloat ParkourTimelineUpdateDelegate;
	FOnTimelineEvent ParkourTimelineFinishDelegate;

	// Parkour params based on parkour mode
	FParkourParams ParkourParams;

	// Pakour position variables by playing parkour aniamtion
	FTransform ParkourTarget;
	FTransform ParkourAnimationStartOffset;
	FTransform ParkourActualStartOffset;

	// Is parkour playing?
	bool bIsPlayingParkour = false;

	// Is parkour cancel time?
	bool bIsPossibleCancelParkour = false;

	// Current playing parkour animation montage
	class UAnimMontage* PlayingParkourAnimMontage;

	// Parkour start location
	FVector ParkourStartLocation;
};