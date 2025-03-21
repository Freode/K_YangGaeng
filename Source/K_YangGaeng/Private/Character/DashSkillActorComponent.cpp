// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Character/DashSkillActorComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundAttenuation.h"

// Sets default values for this component's properties
UDashSkillActorComponent::UDashSkillActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ===== Dash Particle Component Setting =====
	DashFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashParticleSystem"));
	DashFX->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DASHPARTICLE(TEXT("/Game/K_YangGaeng/Characters/FX/NS_Dash_Character.NS_Dash_Character"));
	if (DASHPARTICLE.Succeeded())
	{
		DashFX->SetAsset(DASHPARTICLE.Object);
	}

	// ===== Dash Sound Component Setting =====
	DashSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DashSoundSystem"));
	DashSound->bAutoActivate = false;

	// Set sound asset
	static ConstructorHelpers::FObjectFinder<USoundCue> DASHSOUNDCUE(TEXT("/Game/K_YangGaeng/Characters/Sounds/Cue_Dash_Skill.Cue_Dash_Skill"));
	if (DASHSOUNDCUE.Succeeded())
	{
		DashSound->SetSound(DASHSOUNDCUE.Object);
	}

	// Set sound attenuation
	static ConstructorHelpers::FObjectFinder<USoundAttenuation> DASHSOUNDSA(TEXT("/Game/K_YangGaeng/Characters/Sounds/SA_Dash_Skill.SA_Dash_Skill"));
	if (DASHSOUNDSA.Succeeded())
	{
		DashSound->AttenuationSettings = DASHSOUNDSA.Object;
	}

	// ===== Animation Montage Setting =====

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FORWARDDASHANIMMONTAGE(TEXT("/Game/K_YangGaeng/Characters/Animations/Sequences/AM_Dash_Forward.AM_Dash_Forward"));
	if (FORWARDDASHANIMMONTAGE.Succeeded())
	{
		ForwardDashMontage = FORWARDDASHANIMMONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BACKWARDDASHANIMMONTAGE(TEXT("/Game/K_YangGaeng/Characters/Animations/Sequences/AM_Dash_Backward.AM_Dash_Backward"));
	if (BACKWARDDASHANIMMONTAGE.Succeeded())
	{
		BackwardDashMontage = BACKWARDDASHANIMMONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> LEFTDASHANIMMONTAGE(TEXT("/Game/K_YangGaeng/Characters/Animations/Sequences/AM_Dash_Left.AM_Dash_Left"));
	if (LEFTDASHANIMMONTAGE.Succeeded())
	{
		LeftDashMontage = LEFTDASHANIMMONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RIGHTDASHANIMMONTAGE(TEXT("/Game/K_YangGaeng/Characters/Animations/Sequences/AM_Dash_Right.AM_Dash_Right"));
	if (RIGHTDASHANIMMONTAGE.Succeeded())
	{
		RightDashMontage = RIGHTDASHANIMMONTAGE.Object;
	}

	// Set default's dash power value
	DashPower = 2000.0f;

	// Replicate enable
	SetIsReplicated(true);
}

// Check can operate dash skill?
bool UDashSkillActorComponent::CheckOperateDashSkill(const FVector2D& InDashDirection)
{
	if (bCanOperaterDashSkill)
	{
		// Reset cooldown time
		bCanOperaterDashSkill = false;
		AfterUseDashTime = 0.0f;

		ServerOperateDashSkill(InDashDirection);
		return true;
	}
	else
	{
		return false;
	}
}

// Called when the game starts
void UDashSkillActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// To Server - Execute to can operate dash skill value
void UDashSkillActorComponent::ServerOperateDashSkill_Implementation(const FVector2D& InDashDirection)
{
	MulticastOperateDashSkill(InDashDirection);
	LaunchCharacterFromDashSkill(InDashDirection);
}

// To Server - Execute to end dash skill
void UDashSkillActorComponent::ServerEndDashSkill_Implementation()
{
	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
	K_YG_CHECK(CharacterMovement != nullptr);
	// Prevent to float on air, 
	if (CharacterMovement->MovementMode == EMovementMode::MOVE_Falling)
	{
		Character->LaunchCharacter(FVector(0.0f, 0.0f, -700.0f), false, false);
	}

	// Restore character's gravity
	CharacterMovement->GravityScale = 1.f;
}

// Multicast - Execute to operate dash skill montage
void UDashSkillActorComponent::MulticastOperateDashSkill_Implementation(const FVector2D& InDashDirection)
{
	K_YG_UELOG(Warning, TEXT("Play Dash Skill"));
	DashDirection = InDashDirection;
	PlayDashSkill(InDashDirection);
}

// Called every frame
void UDashSkillActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Calculate dash skill cooldown time
	if (bCanOperaterDashSkill == false)
	{
		AfterUseDashTime = AfterUseDashTime + DeltaTime;
		if (AfterUseDashTime >= DashCooldownTime)
		{
			K_YG_UELOG(Warning, TEXT("Dash Skill Can Use"));
			// Can operate
			bCanOperaterDashSkill = true;
		}
	}
}

/**
*	Play dash skill
*
*	@return Succeed to playing dash skill
*	@param InMoveDirection - Dash move to player input's direction
*/
bool UDashSkillActorComponent::PlayDashSkill(const FVector2D InDashDirection)
{
	// If dash interaction is operating, it will be not operate because of duplication
	K_YG_SIMPLE_CHECK(bIsDashPlaying == false, false);

	// Set playing dash skill
	bIsDashPlaying = true;

	// Y : forward & backward
	// X : right & left
	UAnimMontage* SelectedDashMontage;
	SelectDashSkillMontage(InDashDirection, SelectedDashMontage);

	CheckPlayerCharacter();

	// Play dash skill montage
	Character->GetMesh()->GetAnimInstance()->Montage_Play(SelectedDashMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);
	
	// Play dash niagara particle system
	DashFX->Activate();

	// Active dash sound
	DashSound->Activate();

	return true;
}

// Called when dash skill animation montage is finished to play
void UDashSkillActorComponent::EndDashSkill()
{
	bIsDashPlaying = false;
	
	// Deactive dash niagara particle system
	DashFX->Deactivate();

	if (Character->IsLocallyControlled())
	{
		ServerEndDashSkill();
	}
}

// Attach dash effect on owner's character mesh
void UDashSkillActorComponent::AttachDashEffectOnCharacterMesh()
{
	CheckPlayerCharacter();
	DashFX->SetupAttachment(Character->GetMesh());
	DashFX->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	DashSound->SetupAttachment(Character->GetMesh());
}

/**
*	Select dash skill anim montage which is based on player's input direction
*
*	@param InMoveDirection - Dash move to player input's direction
*	@param OutDashSkillMontage - Selected dash skill anim montage result
*/
void UDashSkillActorComponent::SelectDashSkillMontage(const FVector2D InDashDirection, UAnimMontage* &OutDashSkillMontage)
{
	// Y : forward & backward
	// X : right & left
	// K_YG_UELOG(Warning, TEXT("Move Direction : %s"), *InMoveDirection.ToString());
	// Forward
	if (InDashDirection.Y > KINDA_SMALL_NUMBER)
	{
		OutDashSkillMontage = ForwardDashMontage;
		return;
	}
	// Backward
	else if (InDashDirection.Y < KINDA_SMALL_NUMBER * (-1.0f))
	{
		OutDashSkillMontage = BackwardDashMontage;
		return;
	}

	// Right
	if (InDashDirection.X > KINDA_SMALL_NUMBER)
	{
		OutDashSkillMontage = RightDashMontage;
		return;
	}
	// Left
	else
	{
		OutDashSkillMontage = LeftDashMontage;
		return;
	}
}

// Check player character which is attached this component isn't equal nullptr value
void UDashSkillActorComponent::CheckPlayerCharacter()
{
	if (nullptr == Character)
	{
		Character = Cast<ACharacter>(GetOwner());
	}
}

// Must call run on Server - Launch player character
void UDashSkillActorComponent::LaunchCharacterFromDashSkill(const FVector2D& InDashDirection)
{
	CheckPlayerCharacter();
	// Get player character's direction
	AController* Controller = Cast<AController>(Character->GetController());
	K_YG_CHECK(Controller != nullptr);

	// =================== Calcualte Dash Direction ===================

	// Get character's rotation
	const FRotator CurrentCharacterRotation = Controller->GetControlRotation();
	// Use only yaw rotation data
	const FRotator YawDataRotation = FRotator(0.0f, CurrentCharacterRotation.Yaw, 0.0f);
	// Make rotation matrix and get unit vector x - unit vector x is related with forward and backward
	const FVector ForwardDirection = FRotationMatrix(YawDataRotation).GetUnitAxis(EAxis::X);
	// Make rotation matrix and get unit vector y - unit vector y is related with left and right
	const FVector RightDirection = FRotationMatrix(YawDataRotation).GetUnitAxis(EAxis::Y);

	// Calculate Forward & Right Power
	const FVector ForwardMovePower = ForwardDirection * DashPower * InDashDirection.Y;
	const FVector RightMovePower = RightDirection * DashPower * InDashDirection.X;

	// ================= Modify Dash Power In Specific Situation =================

	// Modify power when player character is moved to cross direction
	FVector TotalMovePower = (FMath::Abs(InDashDirection.X) < KINDA_SMALL_NUMBER && FMath::Abs(InDashDirection.Y) < KINDA_SMALL_NUMBER) ? (ForwardMovePower + RightMovePower) * 0.707f : ForwardMovePower + RightMovePower;

	TotalMovePower = TotalMovePower * DashPowerModification;
	TotalMovePower.Z = 0.0f;

	// Modify power when player falling
	if (Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		TotalMovePower = TotalMovePower * DashFallingPowerModification;
	}

	// When character is dashing, gravity doesn't affect to it
	Character->GetCharacterMovement()->GravityScale = 0.0f;

	// Launch dash skill
	Character->LaunchCharacter(TotalMovePower, false, true);
}

