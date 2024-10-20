// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Character/K_YGOnlyFightCharacter.h"
#include "EnhancedInputComponent.h"
#include "K_YGTestOnlyFightCharacter.generated.h"

class ArrowComponent;
class UOnlyFightInputAComponent;
class AK_YGGun;
class ABaseWeapon; 
class UWeaponActorComponent;

enum class EWeapon : uint8;

/**
 * 
 */


UCLASS()
class K_YANGGAENG_API AK_YGTestOnlyFightCharacter : public AK_YGOnlyFightCharacter
{
	GENERATED_BODY()

public: 
	AK_YGTestOnlyFightCharacter();


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();


	UCameraActorComponent* GetCameraActorComponent() { return CameraComponent; }

	//// the weapon player is having now
	//UPROPERTY(VisibleAnywhere)
	//AK_YGGun* SpawnedGun;

protected:

	//// weapon actor component
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|Weapon", Meta = (AllowPrivateAccess = true))
	//TObjectPtr<UWeaponActorComponent> WeaponComponent;
};
