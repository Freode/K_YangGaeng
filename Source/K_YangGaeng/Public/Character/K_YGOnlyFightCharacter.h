// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Character/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "K_YGOnlyFightCharacter.generated.h"

class UCameraActorComponent;
class UOnlyFightInputAComponent;
class UBaseCharacterAnimInstance;
class UParkourActorComponent;
class UK_YGCharacterMovementComponent;
class UDashSkillActorComponent;
class UOnlyFightAnimActorComponent;
class UWeaponActorComponent;
class AK_YGGun;
class AK_YGHealthBarManager;
class UHealthBarComponent;
class UItemComponent;

enum class EWeapon : uint8;

/**
 * 
 */
UCLASS()
class K_YANGGAENG_API AK_YGOnlyFightCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	AK_YGOnlyFightCharacter();

	// Tick event
	virtual void Tick(float DeltaTime) override;

	// Set player's input configuration
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input - Move
	void InputMove(const FInputActionValue& Value);

	// Input - Sit
	void InputSit(const FInputActionValue& Value);

	// Input - Look ( Mouse Move )
	void InputLook(const FInputActionValue& Value);

	// Input - Jump Start
	void InputJumpStart();

	// Input - Dash Skill
	void InputDashSkill();

	// Send player input movement data
	const FVector2D SendPlayerInputMovementData();

	// Get player input data
	const FVector2D GetPlayerInputMovementData() { return PlayerInputMovementData; }

	// Get Current Anim
	const UBaseCharacterAnimInstance* GetCurrentAnimState() { return CurrentAnim; }

	// Override TakeDamage to handle damage logic
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Set Weapon Anim State
	void SetWeaponState(EWeapon Weaponkind);

protected:

	virtual void BeginPlay() override;

public:

	// The weapon which player is having now
	UPROPERTY(VisibleAnywhere)
	class AK_YGGun* SpawnedGun;

protected:
	
	// Camera custom actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|Character", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraActorComponent> CameraComponent;

	// Input custom actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|Character", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UOnlyFightInputAComponent> InputCustomComponent;

	// Parkour system custom actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|Character", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UParkourActorComponent> ParkourComponent;

	// Dash skill system custom actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|Character", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDashSkillActorComponent> DashSkillComponent;

	// Animation server operations custom actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|Character", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UOnlyFightAnimActorComponent> AnimReplicatedComponent;

	// Weapon actor component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|Weapon", Meta = (AllowPrivateAccess = true))
	TObjectPtr<UWeaponActorComponent> WeaponComponent;

	// Character health widget
	UPROPERTY(VisibleAnywhere) 
	TObjectPtr<UHealthBarComponent> HealthBar;

	// item(iventory) component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemComponent> ItemComponent;

private:

	// Charaacter's custom anim instance class
	UPROPERTY()
	class UBaseCharacterAnimInstance* CurrentAnim;

	// Player's input movement data
	FVector2D PlayerInputMovementData = FVector2D::ZeroVector;

	// Custom character movement component object
	class UK_YGCharacterMovementComponent* AdvancedCharcterMovement;
};
