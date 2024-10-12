// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Character/K_YGOnlyFightCharacter.h"
#include "Character/CameraActorComponent.h"
#include "Character/OnlyFightInputAComponent.h"
#include "Character/BaseCharacterAnimInstance.h"
#include "Character/ParkourActorComponent.h"
#include "Character/K_YGCharacterMovementComponent.h"
#include "Character/DashSkillActorComponent.h"
#include "Character/OnlyFightAnimActorComponent.h"
#include "AI/K_YGAIController.h"
#include "Weapon/WeaponActorComponent.h"
#include "Weapon/K_YGGun.h"
#include "Weapon/ItemComponent.h"
#include "UI/HealthBar/K_YGHealthBarManager.h" 
#include "UI/HealthBar/HealthBarComponent.h"
#include "UI/HealthBar/K_YGHPBarWidget.h"

AK_YGOnlyFightCharacter::AK_YGOnlyFightCharacter()
{
	USkeletalMeshComponent* SKMesh = GetMesh();

	// Set capsule component's collision profile
	UCapsuleComponent* CharacterCapsule = GetCapsuleComponent();
	if (CharacterCapsule != nullptr)
	{
		CharacterCapsule->SetCollisionProfileName(TEXT("K_YG_Character"));
	}

	// Configure character mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CHARACTERMESH(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
	if (CHARACTERMESH.Succeeded())
	{
		SKMesh->SetSkeletalMesh(CHARACTERMESH.Object);
	}

	//if (IsLocallyControlled())
	//{
	//	// Initialize the HealthBarComponent 
	//	HealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HPBARWIDGET"));
	//}

	// Initialize the HealthBarComponent 
	HealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HPBARWIDGET"));

	// Configure detail character's skeletal mesh component
	SKMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	// Create custom camera component and attach to capsule component
	// When component begins CreateDefaultSubobject, the initialization function call immediately
	CameraComponent = CreateDefaultSubobject<UCameraActorComponent>(TEXT("Camera"));
	CameraComponent->ChangeViewMode(EViewMode::THIRD_PERSON);

	// Item (Inventory) componnet
	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("Item"));

	// Create custom input component
	InputCustomComponent = CreateDefaultSubobject<UOnlyFightInputAComponent>(TEXT("InputCustom"));

	// Binding input system in InputCUstomComponent's delegate.
	InputCustomComponent->OnInputMove.BindUObject(this, &AK_YGOnlyFightCharacter::InputMove);
	InputCustomComponent->OnInputStartJump.BindUObject(this, &AK_YGOnlyFightCharacter::InputJumpStart);
	InputCustomComponent->OnInputStopJump.BindUObject(this, &ACharacter::StopJumping);
	InputCustomComponent->OnInputSit.BindUObject(this, &AK_YGOnlyFightCharacter::InputSit);
	InputCustomComponent->OnInputMouseViewChange.BindUObject(this, &AK_YGOnlyFightCharacter::InputLook);
	InputCustomComponent->OnInputDashSkill.BindUObject(this, &AK_YGOnlyFightCharacter::InputDashSkill);

	// === Parkour system custom component ===
	ParkourComponent = CreateDefaultSubobject<UParkourActorComponent>(TEXT("ParkourSystem"));
	ParkourComponent->OnInputCharacterMovingData.BindUObject(this, &AK_YGOnlyFightCharacter::SendPlayerInputMovementData);

	// === Dash Skill system custom component ===
	DashSkillComponent = CreateDefaultSubobject<UDashSkillActorComponent>(TEXT("DashSkillSystem"));
	DashSkillComponent->AttachDashEffectOnCharacterMesh();

	// === Weapon actor custom component & Binding ===
	WeaponComponent = CreateDefaultSubobject<UWeaponActorComponent>(TEXT("WeaponComponent"));
	InputCustomComponent->OnInputStartFire.BindUObject(WeaponComponent, &UWeaponActorComponent::InputStartFire);
	InputCustomComponent->OnInputStopFire.BindUObject(WeaponComponent, &UWeaponActorComponent::InputStopFire);
	InputCustomComponent->OnInputReload.BindUObject(WeaponComponent, &UWeaponActorComponent::InputReload);
	InputCustomComponent->OnInputConvertDischarge.BindUObject(WeaponComponent, &UWeaponActorComponent::InputConvertDischarge);
	InputCustomComponent->OnInputStartAim.BindUObject(WeaponComponent, &UWeaponActorComponent::EnterZoom);
	InputCustomComponent->OnInputStopAim.BindUObject(WeaponComponent, &UWeaponActorComponent::ExitZoom);

	// Weapon item(inventory) select binding 
	InputCustomComponent->OnInputNum1.BindUObject(WeaponComponent, &UWeaponActorComponent::SelectMainWeapon);
	InputCustomComponent->OnInputNum2.BindUObject(WeaponComponent, &UWeaponActorComponent::SelectSubWeapon);


	// === Animation custom setting ===
	SKMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UBaseCharacterAnimInstance> ANIMCLASS(TEXT("/Game/K_YangGaeng/Characters/Animations/AnimBP_OnlyFight.AnimBP_OnlyFight_C"));
	if (ANIMCLASS.Succeeded())
	{
		SKMesh->SetAnimInstanceClass(ANIMCLASS.Class);
	}

	// === Animation replicated system custom component ===
	AnimReplicatedComponent = CreateDefaultSubobject<UOnlyFightAnimActorComponent>(TEXT("AnimReplicatedSystem"));

	// Set character mesh's rotation
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	// Charcter mesh collision setting
	SKMesh->SetCollisionProfileName(TEXT("K_YG_Character_Mesh"));

	// Get custom character movement compoennt
	UK_YGCharacterMovementComponent* CustomCharacterMovement = Cast<UK_YGCharacterMovementComponent>(GetCharacterMovement());
	if (CustomCharacterMovement != nullptr)
	{
		AdvancedCharcterMovement = CustomCharacterMovement;
	}

	// AI Setting
	AIControllerClass = AK_YGAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Tick Event
void AK_YGOnlyFightCharacter::Tick(float DeltaTime)
{
	// Modify pitch range
	float PlayerPitch = GetControlRotation().Pitch;
	if (PlayerPitch > 180.0f)
	{
		PlayerPitch -= 360.0f;
	}

	float PlayerYaw = GetControlRotation().Yaw - GetActorRotation().Yaw;
	if (PlayerYaw > 180.0f)
	{
		PlayerYaw -= 360.0f;
	}
	else if (PlayerYaw < -180.0f)
	{
		PlayerYaw += 360.0f;
	}

	if (IsLocallyControlled())
	{
		AnimReplicatedComponent->ServerNewPlayerData(FTickUpdateData(PlayerInputMovementData, PlayerPitch, PlayerYaw));
	}
}

void AK_YGOnlyFightCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Input system setting in InputCustomComponent
	InputCustomComponent->SetInputSystem(PlayerInputComponent);
}

// Input - Move
void AK_YGOnlyFightCharacter::InputMove(const FInputActionValue& Value)
{
	// Move's input type is Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	PlayerInputMovementData = MovementVector;
	K_YG_CHECK(Controller != nullptr && MovementVector.Size() > KINDA_SMALL_NUMBER);

	// Get character's rotation
	const FRotator CurrentCharacterRotation = Controller->GetControlRotation();
	// Use only yaw rotation data
	const FRotator YawDataRotation = FRotator(0.0f, CurrentCharacterRotation.Yaw, 0.0f);
	// Make rotation matrix and get unit vector x - unit vector x is related with forward and backward
	const FVector ForwardDirection = FRotationMatrix(YawDataRotation).GetUnitAxis(EAxis::X);
	// Make rotation matrix and get unit vector y - unit vector y is related with left and right
	const FVector RightDirection = FRotationMatrix(YawDataRotation).GetUnitAxis(EAxis::Y);

	// Modify movement scale. It is reflected by sitting, running, etc.
	MovementVector = CurrentAnim->ReturnMovementSpeedScaleValue(MovementVector);

	// Key input WASD update to server
	//AnimReplicatedComponent->CheckUpdatePlayerInputWASD(PlayerInputMovementData);

	// Character move - NewValueAxis must be modified another mathematical expression
	if (MovementVector.Y != KINDA_SMALL_NUMBER)
	{
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != KINDA_SMALL_NUMBER)
	{
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// Input - Sit
void AK_YGOnlyFightCharacter::InputSit(const FInputActionValue& Value)
{
	// Sit's input type is bool
	bool bIsSit = Value.Get<bool>();

	// Key input bIsSit update to server
	// Sit -> Idle, Idle -> Sit
	AnimReplicatedComponent->CheckSitInteraction();
}

// Input - Look ( Mouse Move )
void AK_YGOnlyFightCharacter::InputLook(const FInputActionValue& Value)
{
	FVector2D MouseMovingData = Value.Get<FVector2D>();
	
	// Axis X - mouse right(left) moving sensitivity
	if (FMath::Abs(MouseMovingData.X) > KINDA_SMALL_NUMBER)
	{
		float CurrentVelocity = GetCharacterMovement()->Velocity.Size();

		// Character move state is moving or falling
		if (CurrentVelocity > 0.5f)
		{
			// 45.0f must be changed to custom mouse's sensitivity
			AddControllerYawInput(MouseMovingData.X * 45.0f * GetWorld()->GetDeltaSeconds());
		}
		// Character move state is idle
		else
		{
			CameraComponent->CameraLeftRightMoving(MouseMovingData.X * 45.0f * GetWorld()->GetDeltaSeconds());
		}
	}

	// Axis Y - mouse forward(backward) moving sensitivity
	if (FMath::Abs(MouseMovingData.Y) > KINDA_SMALL_NUMBER)
	{
		// 45.0f muse be changed to custom mouse's sensitivity
		CameraComponent->CameraUpDownMoving(MouseMovingData.Y);
	}
}

// Input - Jump Start
void AK_YGOnlyFightCharacter::InputJumpStart()
{
	K_YG_CHECK(CurrentAnim != nullptr);

	// When sit interaciton is activate, jump is changed to sit up interaction
	if (CurrentAnim->CanJumpInteraction() == false)
	{
		AnimReplicatedComponent->CheckSitInteraction();
		return;
	}
	
	// === Parkour Variables ===
	float ParkourHeight;
	FTransform ParkourTargetTransform;
	UPrimitiveComponent* ParkourWallComponent;
	EParkourType ParkourType;

	// Parkour possibility check
	if (ParkourComponent->CheckParkourPossible(ParkourHeight, ParkourTargetTransform, ParkourWallComponent, ParkourType))
	{
		K_YG_SIMPLE(Warning);
		// Parkour Start
		ParkourComponent->StartParkour(ParkourHeight, ParkourTargetTransform, ParkourWallComponent, ParkourType);
	}
	else if (ParkourComponent->GetIsPossibleToCancelParkour())
	{
		K_YG_SIMPLE(Warning);
		ParkourComponent->StopParkour();
	}
	else
	{
		Jump();
	}

}

// Input - Dash Skill
void AK_YGOnlyFightCharacter::InputDashSkill()
{
	K_YG_SIMPLE(Warning);

	// Don't operate when operating parkour interaction
	K_YG_SIMPLE_CHECK(ParkourComponent->GetIsPlayingParkour() == false && CurrentAnim->GetIsMoving());

	// Operate dash skill
	bool bIsDashOperated = DashSkillComponent->CheckOperateDashSkill(PlayerInputMovementData);
	K_YG_SIMPLE_CHECK(bIsDashOperated == true);

	// Cancel sitting
	if (CurrentAnim->GetIsSittingState() || CurrentAnim->GetIsSittingInteraction())
	{
		AnimReplicatedComponent->CheckInitializeSitVariables();
	}
}

// Send player input movement data
const FVector2D AK_YGOnlyFightCharacter::SendPlayerInputMovementData()
{
	return GetPlayerInputMovementData();
}


float AK_YGOnlyFightCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{ 
	HealthBar->MulticastDecreaseHealthWidget(DamageAmount);

	// change real health value
	HealthBar->DecreaseHealth(DamageAmount);
	return DamageAmount;
}

void AK_YGOnlyFightCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get current animation instance immediately
	CurrentAnim = Cast<UBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	
	// Binding function on anim instance's delegate
	CurrentAnim->OnEndDashSkillDelegate.BindUObject(DashSkillComponent, &UDashSkillActorComponent::EndDashSkill);
	CurrentAnim->OnParkourCancelTimeDelegate.BindUObject(ParkourComponent, &UParkourActorComponent::SetParkourCanCancelTime);
	CurrentAnim->OnParkourInteractionFinishDelegate.BindUObject(ParkourComponent, &UParkourActorComponent::ParkourIsFinished);
	CurrentAnim->OnSittingCancelDelegate.BindUObject(AnimReplicatedComponent, &UOnlyFightAnimActorComponent::CheckInitializeSitVariables);

	// Create weapon object 
	// Set which gun will spawn
	//SpawnedGun = WeaponComponent->GetCurrentSelectWeapon();
	SpawnedGun = WeaponComponent->HoldWeapon(NewObject<AK_YGGun>());
	SpawnedGun->SetCameraComponent(CameraComponent);
	WeaponComponent->SetSpawnedGun(SpawnedGun);
	SpawnedGun->SetCollisionOverlap();
}

