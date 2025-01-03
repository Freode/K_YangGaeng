// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"


class AK_YGGun;
class ABaseWeapon;
class UWeaponActorComponent;
struct FWeaponInfo;
enum class EWeapon : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// set weapon actor component
	void SetWeaponComponent(UWeaponActorComponent* CurWeaponComponent) { WeaponComponent = CurWeaponComponent; }


	// Init player's weapon lists
	void InitItem();

	// Pick weapon from map
	void PickWeapon(EWeapon InteractWeaponKind);
	// Drop weapon that current holding
	EWeapon DropWeapon();

	// get currently selected item in inventory
	AK_YGGun* GetCurrentSelectWeapon() { return CurrentPlayerWeaponList[SelectWeaponIndex]; }

	// Weapon list
	TArray<AK_YGGun*> CurrentPlayerWeaponList;
	int16 MaxWeaponListSize;

	// Currently selected weapon index number
	int16 SelectWeaponIndex = 0;

	// Dummy weapon to express empty
	UPROPERTY()
	AK_YGGun* DummyGun;

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	UWeaponActorComponent* WeaponComponent;


};
