﻿// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Character/ParkourActorComponent.h"
#include "System/FunctionLibrary/MathExpansionFunctionLibrary.h"
#include "Curves/CurveVector.h"

// Sets default values for this component's properties
UParkourActorComponent::UParkourActorComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // High parkour animation asset
    static ConstructorHelpers::FObjectFinder<UAnimMontage> HIGHPARKOURANIMATION(TEXT("/Game/K_YangGaeng/Characters/Animations/Sequences/ALS_N_Mantle_2m_Montage_Default.ALS_N_Mantle_2m_Montage_Default"));
    if (HIGHPARKOURANIMATION.Succeeded())
    {
        HighParkourAnimation = HIGHPARKOURANIMATION.Object;
    }

    // Low parkour animation asset
    static ConstructorHelpers::FObjectFinder<UAnimMontage> LOWPARKOURANIMATION(TEXT("/Game/K_YangGaeng/Characters/Animations/Sequences/ALS_N_Mantle_2m_Montage_Default.ALS_N_Mantle_2m_Montage_Default"));
    if (LOWPARKOURANIMATION.Succeeded())
    {
        LowParkourAnimation = LOWPARKOURANIMATION.Object;
    }

    // High parkour curve data
    static ConstructorHelpers::FObjectFinder<UCurveVector> HIGHPARKOURCURVEDATA(TEXT("/Game/K_YangGaeng/Characters/Datas/Parkour/Curve_Parkour_2M.Curve_Parkour_2M"));
    if (HIGHPARKOURCURVEDATA.Succeeded())
    {
        HighParkourCurveVector = HIGHPARKOURCURVEDATA.Object;
    }

    // Low parkour curve data
    static ConstructorHelpers::FObjectFinder<UCurveVector> LOWPARKOURCURVEDATA(TEXT("/Game/K_YangGaeng/Characters/Datas/Parkour/Curve_Parkour_1M.Curve_Parkour_1M"));
    if (LOWPARKOURCURVEDATA.Succeeded())
    {
        LowParkourCurveVector = LOWPARKOURCURVEDATA.Object;
    }

    // Parkour timeline curve data
    static ConstructorHelpers::FObjectFinder<UCurveFloat> PARKOURTIMELINECURVEDATA(TEXT("/Game/K_YangGaeng/Characters/Datas/Parkour/Curve_Parkour_Progress.Curve_Parkour_Progress"));
    if (PARKOURTIMELINECURVEDATA.Succeeded())
    {
        ParkourTimlineCurveData = PARKOURTIMELINECURVEDATA.Object;
    }

    // Create parkour timeline component
    ParkourTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ParkourTimeline"));

}


// Called when the game starts
void UParkourActorComponent::BeginPlay()
{
    Super::BeginPlay();

    // ========== Parkour Timeline ==============

    // Bind functions
    ParkourTimelineUpdateDelegate.BindUFunction(this, FName("ParkourTimelineUpdate"));
    ParkourTimelineFinishDelegate.BindUFunction(this, FName("ParkourTimelineEnd"));

    // Add curve and function to parkour timeline
    ParkourTimeline->AddInterpFloat(ParkourTimlineCurveData, ParkourTimelineUpdateDelegate, FName("Alpha"));
    ParkourTimeline->SetTimelineFinishedFunc(ParkourTimelineFinishDelegate);

    // Set detail option
    ParkourTimeline->SetLooping(false);
    ParkourTimeline->SetIgnoreTimeDilation(true);

    // ==========================================
}


// Called every frame
void UParkourActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

// Check player character which is attached this component isn't equal nullptr value
void UParkourActorComponent::CheckPlayerCharacter()
{
    if (nullptr == Character)
    {
        Character = Cast<ACharacter>(GetOwner());
    }
}

// Get capusle's current world location with Z correction
FVector UParkourActorComponent::GetCapsuleBaseLocation(float ZOffset)
{
    FVector Result = FVector::ZeroVector;

    CheckPlayerCharacter();
    UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent();

    K_YG_CHECK(CharacterCapsule != nullptr, Result);

    // Get current player's half height with up vector
    float HalfHeight = CharacterCapsule->GetScaledCapsuleHalfHeight() + ZOffset;
    Result = CharacterCapsule->GetUpVector() * HalfHeight;

    // Get current player capsule component's bottom world location
    Result = CharacterCapsule->GetComponentLocation() - Result;

    return Result;
}

// Get current character's movement which is equal to player movement input
FVector UParkourActorComponent::GetPlayerMovementInput()
{
    CheckPlayerCharacter();

    FVector Result = FVector::ZeroVector;

    K_YG_CHECK(OnInputCharacterMovingData.IsBound(), Result);

    FVector2D PlayerMoving = OnInputCharacterMovingData.Execute();

    // Get current character's move direction which is player's input
    FRotator ControlRotation = FRotator(0.0f, Character->GetControlRotation().Yaw, 0.0f);
    FVector ForwardVector = UKismetMathLibrary::GetForwardVector(ControlRotation);
    FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);

    // Modify vector's with player input value
    ForwardVector = ForwardVector * PlayerMoving.Y;
    RightVector = RightVector * PlayerMoving.X;

    // Get character's final moving direction
    Result = UKismetMathLibrary::Normal(ForwardVector + RightVector);

    return Result;
}

// Get capsule location from base
FVector UParkourActorComponent::GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset)
{
    CheckPlayerCharacter();
    UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent();

    float HalfHeight = CharacterCapsule->GetScaledCapsuleHalfHeight() + ZOffset;

    return BaseLocation + FVector(0.0f, 0.0f, HalfHeight);
}

/**
* Parkour possiblility checked
*
* @return Can player character do parkour interaction?
* @param OutParkourHeight - Player's character climbed that height
* @param OutParkourWallTransform - Player's character climbed that total transform
* @param OutParkourWallComponent - Wall which is climbed possible
* @param OutParkourType - Play different parkour animation from climbed height
*/
bool UParkourActorComponent::CheckParkourPossible(float& ParkourHeight, FTransform& ParkourWallTransform, UPrimitiveComponent*& ParkourWallComponent, EParkourType& ParkourType)
{
    // Temp setting
    FParkourTraceSetting TraceSetting = FParkourTraceSetting::FParkourTraceSetting(250.0f, 50.0f, 75.0f, 30.0f, 30.0f);
    const EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::ForDuration;
    FVector InitialTraceImpactPoint;
    FVector InitialTraceNormal;
    bool bIsContinueNextStage = false;

    // Checking 1
    bIsContinueNextStage = CheckWallIsFrontOfCharacter(TraceSetting, DebugType, InitialTraceImpactPoint, InitialTraceNormal);
    K_YG_SIMPLE_CHECK(bIsContinueNextStage, false);

    // Checking 2
    FVector DownTraceLocation;
    UPrimitiveComponent* HitComponent;
    bIsContinueNextStage = CheckPlayerIsClimbedWallHeight(TraceSetting, DebugType, InitialTraceImpactPoint, InitialTraceNormal, DownTraceLocation, HitComponent);
    K_YG_SIMPLE_CHECK(bIsContinueNextStage, false);

    // Checking 3
    FTransform TargetTransform;
    float ClimbedHeight;
    bIsContinueNextStage = CheckNotingOverTheWall(DebugType, InitialTraceNormal, DownTraceLocation, TargetTransform, ClimbedHeight);
    K_YG_SIMPLE_CHECK(bIsContinueNextStage, false);

    return true;
}

// 
bool UParkourActorComponent::StartParkour(const float InParkourHeight, const FTransform InParkourWallTransform, const UPrimitiveComponent*& InParkourWallComponent, const EParkourType& InParkourType)
{
    // Step 1.
    FParkourParams ParkourParams = CreateParkourParams(InParkourType, InParkourHeight);
    // Step 2.
    FTransform ParkourWallLocalTransform = InParkourWallTransform;
    ParkourWallLocalTransform = ChangeParkourWallTransformWorldToLocal(InParkourWallComponent, ParkourWallLocalTransform);
    // Step 3.
    FTransform ParkourActualStartOffset = GetActualStartParkourOffset(ParkourWallLocalTransform);
    // Step 4.
    FTransform ParkourAnimationStartOffset = CalculateParkourAnimationStartOffset(ParkourParams, InParkourWallTransform);
    // Step 5.
    CheckPlayerCharacter();
    Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    // Step 6.
    PlayParkourTimeline(ParkourParams);
    // Step 7.

    return false;
}

/**
* Use capsule collision to check wall is in front of character. If it is valid, return wall and capsule collision's interaction location & interaction direction
*
* @return Is specific wall in front of player character?
* @param InTraceSetting - Wall trace base setting values
* @param InDebugType - Is it draw capsule collision's debug?
* @param OutInitialTraceImpactPoint - Wall and capsule collision's interaction location
* @param OutInitialTraceNormal - Wall and capsule collision's interaction direction which is indirect collision to wall
*/
bool UParkourActorComponent::CheckWallIsFrontOfCharacter(const FParkourTraceSetting& InTraceSetting, const EDrawDebugTrace::Type& InDebugType, FVector& OutInitialTraceImpactPoint, FVector& OutInitialTraceNormal)
{
    // Get trace start location which is started character's backward
    // Z is modified to average of checking ledge height
    FVector StartLocation = (GetPlayerMovementInput() * (-30.0f)) + GetCapsuleBaseLocation(2.0f);
    StartLocation.Z = StartLocation.Z + (InTraceSetting.MaxLedgeHeight + InTraceSetting.MinLedgeHeight) * 0.5f;

    // End Location is end of trace
    FVector EndLocation = StartLocation + GetPlayerMovementInput() * InTraceSetting.ForwardReachDistance;

    // Set capsule collision's radius
    float Radius = InTraceSetting.ForwardTraceRadius;

    // Set capsule collision's half height which is length of center
    float HalfHeight = (InTraceSetting.MaxLedgeHeight - InTraceSetting.MinLedgeHeight) * 0.5f + 1.0f;

    // === Prepare capsule trace channel's parameter ===
    FHitResult HitResult;
    // Use custom collision channel
    ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
    bool bTraceComplex = false;
    // Ignore self actor setting
    TArray<AActor*> IgnoreList;
    IgnoreList.Add(GetOwner());
    bool bIgnoreSelf = false;
    FLinearColor TraceColor = FLinearColor::Black;
    FLinearColor TraceHitColor = FLinearColor::Yellow;

    // Activate capsule trace channel
    bool bIsHit = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), StartLocation, EndLocation, Radius, HalfHeight, TraceChannel, bTraceComplex, IgnoreList, InDebugType, HitResult, bIgnoreSelf, TraceColor, TraceHitColor, PARKOUR_DEBUG_DRAW_TIME);

    // Checking
    bool bIsWalkable = Character->GetCharacterMovement()->IsWalkable(HitResult);
    bool bIsBlockingHit = HitResult.bBlockingHit;
    bool bIsInitialOverlap = HitResult.bStartPenetrating;

    // Possible to climb wall
    if (bIsWalkable == false && bIsInitialOverlap == false && bIsBlockingHit)
    {
        OutInitialTraceImpactPoint = HitResult.ImpactPoint;
        OutInitialTraceNormal = HitResult.ImpactNormal;
        return true;
    }
    // Impossible to climb wall
    else
    {
        OutInitialTraceImpactPoint = FVector::ZeroVector;
        OutInitialTraceNormal = FVector::ZeroVector;
        return false;
    }
}


/**
* Use capsule collision to check the wall height whether player's character can climb or not
*
* @return Is the wall's height possible to climb ?
* @param InTraceSetting - Wall trace base setting values
* @param InDebugType - Is it draw capsule collision's debug?
* @param InInitialTraceImpactPoint - Wall and capsule collision's interaction location
* @param InInitialTraceNormal - Wall and capsule collision's interaction direction which is indirect collision to wall
* @param OutDownTraceLocation - Wall's height which is player's character climbed over the wall location
* @param OutHitComponent - Wall which is climbed possible
*/
bool UParkourActorComponent::CheckPlayerIsClimbedWallHeight(const FParkourTraceSetting InTraceSetting, const EDrawDebugTrace::Type InDebugType, const FVector InInitialTraceImpactPoint, const FVector InInitialTraceNormal, FVector& OutDownTraceLocation, UPrimitiveComponent*& OutHitComponent)
{
    // Set interact bottom(end) wall location
    // XY - wall's interaction location XY, Z - character's height
    // Move interaction location to interaction direction
    FVector EndWallLocation = FVector(InInitialTraceImpactPoint.X, InInitialTraceImpactPoint.Y, GetCapsuleBaseLocation(2.0f).Z) + InInitialTraceNormal * (-15.0f);

    // Check wall height
    float WallClimbHeight = InTraceSetting.MaxLedgeHeight + InTraceSetting.DownwardTraceRadius + 1.0f;

    // Set interact top(start) wall location - It is checked from top to bottom
    FVector StartWallLocation = EndWallLocation + FVector(0.0f, 0.0f, WallClimbHeight);

    // === Prepare capsule trace channel's parameter ===
    FHitResult HitResult;
    float Radius = InTraceSetting.DownwardTraceRadius;
    // Use custom collision channel
    ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
    bool bTraceComplex = false;
    // Ignore self actor setting
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(GetOwner());
    bool bIgnoreSelf = false;
    FLinearColor TraceColor = FLinearColor::Red;
    FLinearColor TraceHitColor = FLinearColor::Green;

    // Activate sphere trace channel
    bool bIsHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartWallLocation, EndWallLocation, Radius, TraceChannel, bTraceComplex, IgnoreActors, InDebugType, HitResult, bIgnoreSelf, TraceColor, TraceHitColor, PARKOUR_DEBUG_DRAW_TIME);

    // Player character can climb over the interact wall
    if (HitResult.bBlockingHit && Character->GetCharacterMovement()->IsWalkable(HitResult))
    {
        // Set the wall object and climb over wall location
        OutDownTraceLocation = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.ImpactPoint.Z);
        OutHitComponent = HitResult.GetComponent();
        return true;
    }
    else
    {
        return false;
    }
}

/**
* Use capsule collision to check there is noting on the wall to prevent player character fixed state
*
* @return Is there noting on the wall?
* @param InDebugType - Is it draw capsule collision's debug?
* @param InInitialTraceNormal - Wall and capsule collision's interaction direction which is indirect collision to wall
* @param InDownTraceLocation - Wall's height which is player's character climbed on the wall location
* @param OutTargetTransform - Player's character climbed that total transform
* @param OutClimbedHeight - Player's character climbed that height
*/
bool UParkourActorComponent::CheckNotingOverTheWall(const EDrawDebugTrace::Type InDebugType, const FVector InInitialTraceNormal, const FVector InDownTraceLocation, FTransform& OutTargetTransform, float& OutClimbedHeight)
{
    UCapsuleComponent* CharacterCapsule = Character->GetCapsuleComponent();
    K_YG_CHECK(CharacterCapsule != nullptr, false);

    // Get the character's world location When player's character climbed the wall, 
    FVector TargetLocation = GetCapsuleLocationFromBase(InDownTraceLocation, 2.0f);

    float RadiusOffset = 0.0f;
    float HeightOffset = 0.0f;

    float Height = RadiusOffset * (-1.0f) + HeightOffset + CharacterCapsule->GetScaledCapsuleHalfHeight_WithoutHemisphere();

    // Calculate player character's capsule component property 
    FVector StartLocation = TargetLocation + FVector(0.0f, 0.0f, Height);
    FVector EndLocation = TargetLocation - FVector(0.0f, 0.0f, Height);
    float Radius = CharacterCapsule->GetUnscaledCapsuleRadius() + RadiusOffset;

    // === Prepare capsule trace channel's parameter ===
    FHitResult HitResult;
    // Player character's custom collision profile
    FName ProfileName = FName(TEXT("K_YG_Character"));
    bool bTraceComplex = false;
    // Ignore self
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(GetOwner());
    bool bIgnoreSelf = false;
    FLinearColor TraceColor = FLinearColor::Blue;
    FLinearColor TraceHitColor = FLinearColor(FVector(0.0f, 0.5f, 1.0f));

    // Simulate start
    bool bIsHit = UKismetSystemLibrary::SphereTraceSingleByProfile(GetWorld(), StartLocation, EndLocation, Radius, ProfileName, bTraceComplex, IgnoreActors, InDebugType, HitResult, bIgnoreSelf, TraceColor, TraceHitColor, PARKOUR_DEBUG_DRAW_TIME);

    // Don't overlap or hit anything
    if ((HitResult.bBlockingHit || HitResult.bStartPenetrating) == false)
    {
        FRotator TargetRotation = UKismetMathLibrary::Conv_VectorToRotator(InInitialTraceNormal * FVector(-1.0f, -1.0f, -1.0f));
        OutTargetTransform = UKismetMathLibrary::MakeTransform(TargetLocation, TargetRotation, FVector(1.0f, 1.0f, 1.0f));
        OutClimbedHeight = (TargetLocation - GetOwner()->GetActorLocation()).Z;

        return true;
    }
    else
    {
        return false;
    }
}

// Starting position and play rate change new range based on climbed height 
FParkourParams UParkourActorComponent::CreateParkourParams(const EParkourType& InParkourType, const float& InClimbedHeight)
{
    // Get parkour asset setting from parkour type
    FParkourAssetSetting BaseParkourAssetSetting = GetParkourAssetSetting(InParkourType);

    // === Parkour params setting ===
    UAnimMontage* AnimMontage = BaseParkourAssetSetting.AnimMontage;
    UCurveVector* CurveVector = BaseParkourAssetSetting.PositionAndCorrectionCurve;
    FVector StartingOffset = BaseParkourAssetSetting.StartingOffset;

    //
    float StartingPosition = UKismetMathLibrary::MapRangeClamped(InClimbedHeight, BaseParkourAssetSetting.LowHeight, BaseParkourAssetSetting.HighHeight, BaseParkourAssetSetting.LowStartPosition, BaseParkourAssetSetting.HighStartPosition);
    // 
    float PlayRate = UKismetMathLibrary::MapRangeClamped(InClimbedHeight, BaseParkourAssetSetting.LowHeight, BaseParkourAssetSetting.HighHeight, BaseParkourAssetSetting.LowPlayRate, BaseParkourAssetSetting.HighPlayRate);

    FParkourParams NewParkourParams = FParkourParams(AnimMontage, CurveVector, StartingOffset, StartingPosition, PlayRate);

    return NewParkourParams;
}

// Get parkour asset setting by parkour type
FParkourAssetSetting UParkourActorComponent::GetParkourAssetSetting(const EParkourType& InParkourType)
{
    FParkourAssetSetting ParkourAssetSetting;

    switch (InParkourType)
    {
        // Low parkour animation setting
    case EParkourType::LOW_PARKOUR:
        ParkourAssetSetting.AnimMontage = LowParkourAnimation;
        ParkourAssetSetting.PositionAndCorrectionCurve = LowParkourCurveVector;
        ParkourAssetSetting.StartingOffset = FVector(0.0f, 65.0f, 200.0f);
        ParkourAssetSetting.LowHeight = 50.0f;
        ParkourAssetSetting.LowPlayRate = 1.0f;
        ParkourAssetSetting.LowStartPosition = 0.5f;
        ParkourAssetSetting.HighHeight = 100.0f;
        ParkourAssetSetting.HighPlayRate = 1.0f;
        ParkourAssetSetting.HighStartPosition = 0.0f;
        break;

        // High parkour animation setting
    case EParkourType::HIGH_PARKOUR:
        ParkourAssetSetting.AnimMontage = HighParkourAnimation;
        ParkourAssetSetting.PositionAndCorrectionCurve = HighParkourCurveVector;
        ParkourAssetSetting.StartingOffset = FVector(0.0f, 65.0f, 200.0f);
        ParkourAssetSetting.LowHeight = 125.0f;
        ParkourAssetSetting.LowPlayRate = 1.2f;
        ParkourAssetSetting.LowStartPosition = 0.6f;
        ParkourAssetSetting.HighHeight = 200.0f;
        ParkourAssetSetting.HighPlayRate = 1.2f;
        ParkourAssetSetting.HighStartPosition = 0.0f;
        break;
    }

    return ParkourAssetSetting;
}

// Parkour wall change transform from world to local
FTransform UParkourActorComponent::ChangeParkourWallTransformWorldToLocal(const UPrimitiveComponent* InComponent, FTransform& InTransform)
{
    return UMathExpansionFunctionLibrary::ChangeComponentTransformWorldToLocal(InComponent, InTransform);
}

// Return actual start parkour offset
FTransform UParkourActorComponent::GetActualStartParkourOffset(const FTransform& InTargetTransform)
{
    return UMathExpansionFunctionLibrary::TransformMinusOperation(GetOwner()->GetActorTransform(), InTargetTransform);
}

// Return calculate parkour animation start offset
FTransform UParkourActorComponent::CalculateParkourAnimationStartOffset(const FParkourParams& InParkourParams, const FTransform& InTargetTransform)
{
    // The location which player character is climbed target is modified offset distance to wall inner direction 
    FVector TargetDirection = UKismetMathLibrary::Conv_RotatorToVector(InTargetTransform.GetRotation().Rotator());

    // Climbed target location - XY : 65, Z : 200
    FVector ModifiedLocation = InTargetTransform.GetLocation() - FVector(TargetDirection.X, TargetDirection.Y, InParkourParams.StartingOffset.Z);

    // Calculate start parkour animation offset and return it
    FTransform TempModifiedTransform = UKismetMathLibrary::MakeTransform(ModifiedLocation, InTargetTransform.GetRotation().Rotator(), FVector(1.0f, 1.0f, 1.0f));
    return UMathExpansionFunctionLibrary::TransformMinusOperation(TempModifiedTransform, InTargetTransform);
}

// Start Parkour Step 6. Play parkour timeline
void UParkourActorComponent::PlayParkourTimeline(const FParkourParams& InParkourParams)
{
    // Get timeline max length
    float TimelineMinLength, TimelineMaxLength;
    InParkourParams.PositionAndCorrectionCurve->GetTimeRange(TimelineMinLength, TimelineMaxLength);

    // Modify start position with parkour param's start position offset
    TimelineMaxLength = TimelineMaxLength - InParkourParams.StartingPosition;

    // Set parkour timeline details
    ParkourTimeline->SetTimelineLength(TimelineMaxLength);
    ParkourTimeline->SetPlayRate(InParkourParams.PlayRate);

    // Play parkour timeline
    ParkourTimeline->PlayFromStart();
}

// Start Parkour Step 7. Play parkour animation
bool UParkourActorComponent::PlayParkourAnimation(const FParkourParams& InParkourParams)
{
    // Checking anim montage is valid
    K_YG_SIMPLE_CHECK(InParkourParams.AnimMontage != nullptr, false);

    // Checking player character's anim instance is valid
    UAnimInstance* CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
    K_YG_SIMPLE_CHECK(CharacterAnimInstance != nullptr, false);

    // Play parkour anim montage
    CharacterAnimInstance->Montage_Play(InParkourParams.AnimMontage, InParkourParams.PlayRate, EMontagePlayReturnType::MontageLength, InParkourParams.StartingPosition, false);

    return true;
}

// Execute every one frame(tick) when parkour timeline is progressing(update)
void UParkourActorComponent::ParkourTimelineUpdate(float Value)
{

}

// Execute when parkour timeline progress finish
void UParkourActorComponent::ParkourTimelineEnd()
{

}