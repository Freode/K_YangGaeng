// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/ItemComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/K_YGGun.h"
#include "Weapon/WeaponActorComponent.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Character = Cast<ACharacter>(GetOwner());

	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// set dummy gun's weapon kind 'NONE' and treat like empty 
	DummyGun = NewObject<AK_YGGun>(this);
	DummyGun->WeaponInfo.WeaponKind = EWeapon::NONE;

	MaxWeaponListSize = 5;
	InitItem();
	// ...
	
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemComponent::InitItem()
{
	CurrentPlayerWeaponList.Init(DummyGun, MaxWeaponListSize);
}

void UItemComponent::PickWeapon(EWeapon InteractWeaponKind)
{ 
	for (int i = 0; i < MaxWeaponListSize; i++)
	{
		// if current space is empty, pick weaon in it
		if (CurrentPlayerWeaponList[i]->WeaponInfo.WeaponKind == EWeapon::NONE)
		{ 
			CurrentPlayerWeaponList[i] = WeaponComponent->GetWeapon(InteractWeaponKind);
		}
	}
}

// Drop currently selected weapon and return it
EWeapon UItemComponent::DropWeapon()
{
	EWeapon DropWeaponKind;
	DropWeaponKind = CurrentPlayerWeaponList[SelectWeaponIndex]->WeaponInfo.WeaponKind;
	CurrentPlayerWeaponList[SelectWeaponIndex] = DummyGun;

	return DropWeaponKind;
}

