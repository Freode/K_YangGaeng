// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/WeaponActorComponent.h"
#include "Weapon/K_YGTestOnlyFightCharacter.h"
#include "Weapon/K_YGHandGun.h"
#include "Weapon/K_YGSniperRifle.h"
#include "Weapon/K_YGRifle.h"
#include "Weapon/K_YGShotGun.h"
#include "Weapon/K_YGSMG.h"
#include "Weapon/K_YGGun.h"
#include "Character/OnlyFightInputAComponent.h"


// Sets default values for this component's properties
UWeaponActorComponent::UWeaponActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	InitItemList();
}


// Called when the game starts
void UWeaponActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//SpawnedGun = GetWeapon(EWeapon::NONE);
	DummyGun = GetWeapon(EWeapon::NONE);;
	InitItemList();
}

void UWeaponActorComponent::StuffWeaponIsList(EWeapon WeaponKind)
{
	// check if list is full , if not then find empty space at list to stuff in it
	if (MaxWeaponListSize <= CurrentWeaponCount)
	{
		K_YG_UELOG(Display, TEXT("weapon list is full"));
		return;
	}

	for (int i = 0; i < MaxWeaponListSize; i++)
	{
		// if current space is empty, pick weaon in it
		if (!CurrentPlayerWeaponList[i])
		{
			K_YG_UELOG(Error, TEXT("Current Player Weapon List's index %d is null"), i);
			return;
		}
		if (CurrentPlayerWeaponList[i]->WeaponInfo.WeaponKind == EWeapon::NONE)
		{
			CurrentWeaponCount++;
			K_YG_UELOG(Display, TEXT("Weapon Pick Success. Current Weapon count: %d"), CurrentWeaponCount);
			CurrentPlayerWeaponList[i] = GetWeapon(WeaponKind);

			// hold weapon just before picked 
			SelectWeaponIndex = i;
			ServerPickWeapon(WeaponKind);
			return;
		}
	}
}

void UWeaponActorComponent::ServerInputStartFire_Implementation()
{
	MulticastInputStartFire();
}

void UWeaponActorComponent::ServerInputStopFire_Implementation()
{
	MulticastInputStopFire();
}

void UWeaponActorComponent::ServerInputReload_Implementation()
{
	MulticastInputReload();
}

void UWeaponActorComponent::ServerEnterZoom_Implementation()
{
	MulticastEnterZoom();
}

void UWeaponActorComponent::ServerExitZoom_Implementation()
{
	MulticastExitZoom();
}

void UWeaponActorComponent::ServerConvertDischarge_Implementation()
{
	MulticastConvertDischarge();
}

void UWeaponActorComponent::ServerGiveDamage_Implementation(float Damage)
{
}

void UWeaponActorComponent::ServerUpdateCurrentWeapon_Implementation()
{
	MulticastUpdateCurrentWeapon();
}

void UWeaponActorComponent::ServerPickWeapon_Implementation(EWeapon InteractWeaponKind)
{
	K_YG_SIMPLE(Display);
	MulticastPickWeapon(InteractWeaponKind);
}

void UWeaponActorComponent::ServerDropWeapon_Implementation()
{
	MulticastDropWeapon();
}

void UWeaponActorComponent::ServerSelectNum1_Implementation(FWeaponInfo SelectGunInfo)
{
	K_YG_SIMPLE(Display);

	MulticastSelectNum1(SelectGunInfo);
}

void UWeaponActorComponent::ServerSelectNum2_Implementation(FWeaponInfo SelectGunInfo)
{
	MulticastSelectNum2(SelectGunInfo);
}

 



// Start fire with mouse left key pressed
void UWeaponActorComponent::MulticastInputStartFire_Implementation()
{ 
	if (!SpawnedGun)
	{
		K_YG_UELOG(Error, TEXT("SpawnedGun is nullptr"));
		return;
	}

	// if players crosshair directing weapon not enemy, then don't fire and pick a selected weapon
	if (SpawnedGun->CheckHoveringWeapon())
	{  
		StuffWeaponIsList(SpawnedGun->PickHoveredWeapon()->WeaponInfo.WeaponKind);
		return; 
	}
	
	if ( SpawnedGun->WeaponInfo.WeaponKind == EWeapon::NONE) {
		K_YG_UELOG(Warning, TEXT("There is no Gun Instance"));
		return;
	}
	SpawnedGun->EnterFire();

}

// Start fire with mouse left key released
void UWeaponActorComponent::MulticastInputStopFire_Implementation()
{
	if (!SpawnedGun)
	{
		K_YG_UELOG(Error, TEXT("SpawnedGun is nullptr"));
		return;
	}
	if (SpawnedGun->WeaponInfo.WeaponKind == EWeapon::NONE) {
		K_YG_UELOG(Error, TEXT("There is no Gun Instance"));
		return;
	}
	SpawnedGun->ExitFire();

}


void UWeaponActorComponent::InputStartFire()
{
	ServerInputStartFire();
 
}

void UWeaponActorComponent::InputStopFire()
{
	ServerInputStopFire();
}

void UWeaponActorComponent::InputReload()
{
	ServerInputReload();
}

// Start fire with R key pressed
void UWeaponActorComponent::MulticastInputReload_Implementation()
{
	SpawnedGun->EnterReload();

	if (SpawnedGun->UpperBodyReload == nullptr)
	{
		K_YG_UELOG(Error, TEXT("UpperBodyIdle montage is nullptr"));
		return;
	}

}

void UWeaponActorComponent::MulticastEnterZoom_Implementation()
{

	SpawnedGun->EnterAim();
}

void UWeaponActorComponent::MulticastExitZoom_Implementation()
{
	SpawnedGun->ExitAim();
}

void UWeaponActorComponent::MulticastConvertDischarge_Implementation()
{
	SpawnedGun->ConvertDisChargeModeAll();
}

void UWeaponActorComponent::MulticastGiveDamage_Implementation(float Damage)
{
}

void UWeaponActorComponent::MulticastUpdateCurrentWeapon_Implementation()
{
	SpawnedGun = GetCurrentSelectWeapon();
}

void UWeaponActorComponent::MulticastPickWeapon_Implementation(EWeapon InteractWeaponKind)
{  
	//// check if list is full , if not then find empty space at list to stuff in it
	//if (MaxWeaponListSize <= CurrentWeaponCount)
	//{
	//	K_YG_UELOG(Display, TEXT("weapon list is full"));
	//	return;
	//}

	//for (int i = 0; i < MaxWeaponListSize; i++)
	//{
	//	// if current space is empty, pick weaon in it
	//	if (!CurrentPlayerWeaponList[i])
	//	{
	//		K_YG_UELOG(Error, TEXT("Current Player Weapon List's index %d is null"), i);
	//		return;
	//	}
	//	if (CurrentPlayerWeaponList[i]->WeaponInfo.WeaponKind == EWeapon::NONE)
	//	{ 
	//		CurrentWeaponCount++;
	//		K_YG_UELOG(Display, TEXT("Weapon Pick Success. Current Weapon count: %d"), CurrentWeaponCount);
	//		CurrentPlayerWeaponList[i] = GetWeapon(InteractWeaponKind);

	//		HoldWeapon(GetCurrentSelectWeapon());
	//		return;
	//	}
	//}
	if (Character->IsLocallyControlled())
	{
		HoldWeapon(GetCurrentSelectWeapon());
	}
	else
	{
		HoldWeapon(GetWeapon(InteractWeaponKind));
	}
}

void UWeaponActorComponent::MulticastDropWeapon_Implementation()
{
	if (!Character->IsLocallyControlled())
	{
		HoldWeapon(DummyGun);
		return;
	}
	//EWeapon DropWeaponKind;
	//DropWeaponKind = CurrentPlayerWeaponList[SelectWeaponIndex]->WeaponInfo.WeaponKind;
	if (CurrentPlayerWeaponList[SelectWeaponIndex]->WeaponInfo.WeaponKind == EWeapon::NONE)
	{
		K_YG_UELOG(Display, TEXT("can't drop weapon: already empty"));
		return;
	}
	CurrentPlayerWeaponList[SelectWeaponIndex] = DummyGun;
	EWeaponToString(DummyGun->WeaponInfo.WeaponKind);
	HoldWeapon(DummyGun);
	CurrentWeaponCount--;

	//return DropWeaponKind;
}

void UWeaponActorComponent::MulticastSelectNum1_Implementation(FWeaponInfo SelectGunInfo)
{ 
	K_YG_SIMPLE(Display);
	SelectMainWeapon(SelectGunInfo);
}

void UWeaponActorComponent::MulticastSelectNum2_Implementation(FWeaponInfo SelectGunInfo)
{
	SelectSubWeapon(SelectGunInfo);
}

void UWeaponActorComponent::EWeaponToString(EWeapon WeaponKind)
{
	switch (WeaponKind)
	{
	case EWeapon::NONE:
		K_YG_UELOG(Display, TEXT("None"));
		break;
	case EWeapon::HANDGUN:
		K_YG_UELOG(Display, TEXT("Hand Gun"));
		break;

	case EWeapon::RIFLE:
		K_YG_UELOG(Display, TEXT("Rifle"));
		break;

	case EWeapon::SNIPERRIFLE:
		K_YG_UELOG(Display, TEXT("Sniper Rifle"));
		break;

	case EWeapon::SMG:
		K_YG_UELOG(Display, TEXT("SMG"));
		break;

	case EWeapon::SHOTGUN:
		K_YG_UELOG(Display, TEXT("Shot Gun"));
		break;

	default:
		K_YG_UELOG(Display, TEXT("ect."));
		break;
	}
}

 

// Start fire with mouse right key pressed
void UWeaponActorComponent::EnterZoom()
{
	ServerEnterZoom();
}

// Start fire with mouse left key released
void UWeaponActorComponent::ExitZoom()
{
	ServerExitZoom();
}

// Start fire with mouse left key
void UWeaponActorComponent::InputConvertDischarge()
{
	ServerConvertDischarge();
}


// step 1 - get weapon object from list that currently selected using 'GetCurrentSelectWeapon()'
// step 2 - spawn weapon actor in map and attach to player using 'HoldWeapon()'
void UWeaponActorComponent::SelectMainWeapon(FWeaponInfo SelectGunInfo)
{  
	SelectWeaponIndex = 0;
	HoldWeapon(GetWeapon(SelectGunInfo.WeaponKind));
	K_YG_UELOG(Display, TEXT("Select Main Weapon"));
}

void UWeaponActorComponent::SelectSubWeapon(FWeaponInfo SelectGunInfo)
{ 
	SelectWeaponIndex = 1;
	HoldWeapon(GetWeapon(SelectGunInfo.WeaponKind));
	K_YG_UELOG(Display, TEXT("Select Sub Weapon"));
}

void UWeaponActorComponent::SelectNum1()
{
	ServerSelectNum1(GetCurrentSelectWeapon()->WeaponInfo);
}

void UWeaponActorComponent::SelectNum2()
{
	ServerSelectNum2(GetCurrentSelectWeapon()->WeaponInfo);
}




void UWeaponActorComponent::CheckPlayerCharacter()
{
	if (nullptr == Character)
	{
		Character = Cast<ACharacter>(GetOwner());
	}
 


}

void UWeaponActorComponent::BindInput(UOnlyFightInputAComponent* InputCustomComponent)
{
	InputCustomComponent->OnInputStartFire.BindUObject(this, &UWeaponActorComponent::InputStartFire);
	InputCustomComponent->OnInputStopFire.BindUObject(this, &UWeaponActorComponent::InputStopFire);
	InputCustomComponent->OnInputReload.BindUObject(this, &UWeaponActorComponent::InputReload);
	InputCustomComponent->OnInputConvertDischarge.BindUObject(this, &UWeaponActorComponent::InputConvertDischarge);
	InputCustomComponent->OnInputStartAim.BindUObject(this, &UWeaponActorComponent::EnterZoom);
	InputCustomComponent->OnInputStopAim.BindUObject(this, &UWeaponActorComponent::ExitZoom);
}

void UWeaponActorComponent::InitItemList()
{
	CurrentPlayerWeaponList.Init(DummyGun, MaxWeaponListSize);
}

void UWeaponActorComponent::DropWeapon()
{
	ServerDropWeapon();
}



// update weapon info before player select other weapon 
void UWeaponActorComponent::UpdateWeaponInfo(int16 CurrentWeaponListIndex)
{
	CurrentPlayerWeaponList[CurrentWeaponListIndex]->WeaponInfo = SpawnedGun->WeaponInfo;
}

 

// Called every frame
void UWeaponActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// create new weapon depending on 'WeaponKind' and return it
//AK_YGGun* UWeaponActorComponent::GetWeapon(EWeapon WeaponKind)
//{	
//	AK_YGGun* NewWeapon = NewObject<AK_YGGun>();
//	CheckPlayerCharacter();
//	if (Character == nullptr)
//	{
//		K_YG_UELOG(Error, TEXT("Player character is nullptr"));
//		return nullptr;
//	}
//	FName WeaponSocket;
//	switch (WeaponKind)
//	{
//		// create AK_YGGun(no static mesh) when weapon kind is NONE
//		// no static mesh and just has only function 
//	case EWeapon::NONE:
//		K_YG_UELOG(Warning, TEXT("Create Nothing"));
//		NewWeapon = Character->GetWorld()->SpawnActor<AK_YGGun>();
//		break;
//		// create sniper rifle object
//	case EWeapon::SNIPERRIFLE:
//		WeaponSocket = TEXT("hand_rSocket_SniperRifle");
//		NewWeapon = Character->GetWorld()->SpawnActor<AK_YGSniperRifle>();
//		break;
//
//		// create SMG object
//	case EWeapon::SMG:
//		WeaponSocket = TEXT("hand_rSocket_SMG");
//		NewWeapon = Character->GetWorld()->SpawnActor<AK_YGSMG>();
//		break;
//
//		// create rifle object
//	case EWeapon::RIFLE:
//		WeaponSocket = TEXT("hand_rSocket_RifleIronSight");
//		NewWeapon = Character->GetWorld()->SpawnActor<AK_YGRifle>();
//		break;
//
//		// create handgun object
//	case EWeapon::HANDGUN:
//		WeaponSocket = TEXT("hand_rSocket_HandGun");
//		NewWeapon = Character->GetWorld()->SpawnActor<AK_YGHandGun>();
//		break;
//
//		// create shotgun object
//	case EWeapon::SHOTGUN:
//		WeaponSocket = TEXT("hand_rSocket_ShotGunIronSight");
//		NewWeapon = Character->GetWorld()->SpawnActor<AK_YGShotGun>();
//		break;
//	case EWeapon::GRENADE:
//		break;
//	case EWeapon::KNIFE:
//		break;
//
//	default:
//		WeaponSocket = TEXT("hand_rSocket_SniperRifle");
//		NewWeapon = Character->GetWorld()->SpawnActor<AK_YGGun>();
//		break;
//	}
//
//	if (NewWeapon)
//	{
//		NewWeapon->SetOwnerCharacter(Character);
//		NewWeapon->InitCharacterInstance();
//		NewWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
//		K_YG_UELOG(Error, TEXT("Test Gun Create "));
//	}
//
//	 
//	return NewWeapon;
//}
AK_YGGun* UWeaponActorComponent::GetWeapon(EWeapon WeaponKind)
{
	AK_YGGun* NewWeapon = NewObject<AK_YGGun>();
	CheckPlayerCharacter(); 

	switch (WeaponKind)
	{
		// create AK_YGGun(no static mesh) when weapon kind is NONE
		// no static mesh and just has only function 
	case EWeapon::NONE:
		K_YG_UELOG(Warning, TEXT("Create Nothing"));
		NewWeapon = NewObject<AK_YGGun>();
		break;

		// create sniper rifle object
	case EWeapon::SNIPERRIFLE:
		NewWeapon = NewObject<AK_YGSniperRifle>();
		break;

		// create SMG object
	case EWeapon::SMG:
		NewWeapon = NewObject<AK_YGSMG>();
		break;

		// create rifle object
	case EWeapon::RIFLE:
		NewWeapon = NewObject<AK_YGRifle>();
		break;

		// create handgun object
	case EWeapon::HANDGUN:
		NewWeapon = NewObject<AK_YGHandGun>();
		break;

		// create shotgun object
	case EWeapon::SHOTGUN:;
		NewWeapon = NewObject<AK_YGShotGun>();
		break;

	case EWeapon::GRENADE:
		break;
	case EWeapon::KNIFE:
		break;

	default:
		NewWeapon = Character->GetWorld()->SpawnActor<AK_YGGun>();
		break;
	}

	if (NewWeapon)
	{
		NewWeapon->SetOwnerCharacter(Character);
		NewWeapon->InitCharacterInstance();
	}


	return NewWeapon;
}

AK_YGGun* UWeaponActorComponent::HoldWeapon(AK_YGGun* WeaponToHold)
{
	// destroy weapon that player had ealry held
	if (SpawnedGun)
	{
		SpawnedGun->Destroy();
	}

	AK_YGGun* NewWeapon = NewObject<AK_YGGun>();
	if (!WeaponToHold)
	{
		K_YG_UELOG(Error, TEXT("Weapon To hold is null ptr"));
		return nullptr;
	}
	EWeapon WeaponKind = WeaponToHold->WeaponInfo.WeaponKind;
	CheckPlayerCharacter();
	if (Character == nullptr)
	{
		K_YG_UELOG(Error, TEXT("Player character is nullptr"));
		return nullptr;
	} 
	FName WeaponSocket;
	switch (WeaponKind)
	{
		// create AK_YGGun(no static mesh) when weapon kind is NONE
		// no static mesh and just has only function 
	case EWeapon::NONE:
		K_YG_UELOG(Display, TEXT("Create Nothing"));
		SpawnedGun = DummyGun;
		DummyGun->ReturnMontageNull();
		break;
		// create sniper rifle object
	case EWeapon::SNIPERRIFLE:
		WeaponSocket = TEXT("hand_rSocket_SniperRifle");
		SpawnedGun = Character->GetWorld()->SpawnActor<AK_YGSniperRifle>();
		break;

		// create SMG object
	case EWeapon::SMG:
		WeaponSocket = TEXT("hand_rSocket_SMG");
		SpawnedGun = Character->GetWorld()->SpawnActor<AK_YGSMG>();
		break;

		// create rifle object
	case EWeapon::RIFLE:
		WeaponSocket = TEXT("hand_rSocket_RifleIronSight");
		SpawnedGun = Character->GetWorld()->SpawnActor<AK_YGRifle>();
		break;

		// create handgun object
	case EWeapon::HANDGUN:
		WeaponSocket = TEXT("hand_rSocket_HandGun");
		SpawnedGun = Character->GetWorld()->SpawnActor<AK_YGHandGun>();
		break;

		// create shotgun object
	case EWeapon::SHOTGUN:
		WeaponSocket = TEXT("hand_rSocket_ShotGunIronSight");
		SpawnedGun = Character->GetWorld()->SpawnActor<AK_YGShotGun>();
		break;
	case EWeapon::GRENADE:
		break;
	case EWeapon::KNIFE:
		break;

	default:
		WeaponSocket = TEXT("hand_rSocket_SniperRifle");
		SpawnedGun = Character->GetWorld()->SpawnActor<AK_YGGun>();
		break;
	}

	if (SpawnedGun)
	{
		SpawnedGun->SetOwnerCharacter(Character);
		SpawnedGun->InitCharacterInstance();
		SpawnedGun->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		K_YG_UELOG(Error, TEXT("Test Gun Create "));
		// get original weapon info from inventory 
		SpawnedGun->WeaponInfo = WeaponToHold->WeaponInfo;

		// set collision overlap to prevent player character move weird
		SpawnedGun->SetCollisionOverlap();
		SpawnedGun->SetCameraComponent(GetCameraComponent());

		Cast<AK_YGOnlyFightCharacter>(Character)->SetWeaponState(WeaponKind);
		//SetAnimWeaponState(WeaponKind);

	}


	return SpawnedGun;
}
