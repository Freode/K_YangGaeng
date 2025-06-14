// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/WeaponActorComponent.h"
#include "Weapon/K_YGTestOnlyFightCharacter.h"
#include "Weapon/K_YGHandGun.h"
#include "Weapon/K_YGSniperRifle.h"
#include "Weapon/K_YGRifle.h"
#include "Weapon/K_YGShotGun.h"
#include "Weapon/K_YGSMG.h"
#include "Weapon/K_YGGun.h"
#include "UI/GunInterface/GunInterfaceComponent.h"
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
	 
	// player controller
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// player skeletalmesh
	if (Character)
	{
		SKMesh = Character->GetMesh();
	}
	else
	{
		K_YG_UELOG(Display, TEXT("no skeletal mesh "));
	}
}

bool UWeaponActorComponent::StuffWeaponIsList(EWeapon WeaponKind)
{
	// check if list is full , if not then find empty space at list to stuff in it
	if (MaxWeaponListSize <= CurrentWeaponCount)
	{
		K_YG_UELOG(Display, TEXT("weapon list is full"));
		return false;
	}

	for (int i = 0; i < MaxWeaponListSize; i++)
	{
		// if current space is empty, pick weaon in it
		if (!CurrentPlayerWeaponList[i])
		{
			K_YG_UELOG(Error, TEXT("Current Player Weapon List's index %d is null"), i);
			return false;
		}
		if (CurrentPlayerWeaponList[i]->WeaponInfo.WeaponKind == EWeapon::NONE)
		{
			CurrentWeaponCount++;
			K_YG_UELOG(Display, TEXT("Weapon Pick Success. Current Weapon count: %d"), CurrentWeaponCount);
			CurrentPlayerWeaponList[i] = GetWeapon(WeaponKind);


			// change widget iamge's opacity to 50% (unselect) 
			GunInterface->SetWeaponImage(SelectWeaponIndex, SpawnedGun->WeaponInfo.WeaponKind, 0.5);
			// hold weapon just before picked 
			SelectWeaponIndex = i;
				 
			ServerPickWeapon(WeaponKind);

			return true;
		}
	}

	return false;
}

void UWeaponActorComponent::SaveWeaponInfoInList(FWeaponInfo InWeaponInfo)
{
	EWeaponToString(InWeaponInfo.WeaponKind);
	// set weapon info in list
	CurrentPlayerWeaponList[SelectWeaponIndex]->WeaponInfo = InWeaponInfo;

	// change widget iamge's opacity to 50% (unselect)
	GunInterface->SetWeaponImage(SelectWeaponIndex, InWeaponInfo.WeaponKind, 0.5);
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

void UWeaponActorComponent::ServerSelectNum3_Implementation(FWeaponInfo SelectGunInfo)
{
	MulticastSelectNum3(SelectGunInfo);
}

void UWeaponActorComponent::ServerDestroyWeapon_Implementation(AK_YGGun* InWeaponToDestroy)
{
	MulticastDestroyWeapon(InWeaponToDestroy);
}

 



// Start fire with mouse left key pressed
void UWeaponActorComponent::MulticastInputStartFire_Implementation()
{  
	// player has nothing
	if (!SpawnedGun)
	{ 
		return;
	}

	// player activating shop widget
	if (Cast<AK_YGOnlyFightCharacter>(Character)->bIsWidgetActivated)
	{ 
		return;
	} 

	// if players crosshair directing weapon not enemy, then don't fire and pick a selected weapon
	if (SpawnedGun->CheckHoveringWeapon())
	{  
		// get item
		StuffWeaponIsList(SpawnedGun->PickHoveredWeapon()->WeaponInfo.WeaponKind);
		// after get item, destoy that object in game map
		//SpawnedGun->PickHoveredWeapon()->Destroy();
		ServerDestroyWeapon(SpawnedGun->PickHoveredWeapon()); 
		return; 
	}
	
	if ( SpawnedGun->WeaponInfo.WeaponKind == EWeapon::NONE) 
	{ 
		return;
	}
	 
	SpawnedGun->EnterFire();

}

// Start fire with mouse left key released
void UWeaponActorComponent::MulticastInputStopFire_Implementation()
{
	if (!SpawnedGun)
	{ 
		return;
	}
	if (SpawnedGun->WeaponInfo.WeaponKind == EWeapon::NONE) { 
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
	EWeapon WeaponKind = SpawnedGun->WeaponInfo.WeaponKind;
	SpawnedGun->EnterReload();
	SpawnedGun->WeaponInfo.WeaponKind = WeaponKind;

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
	if (CurrentWeaponCount < 0)
	{	
		CurrentWeaponCount = 0;
		K_YG_UELOG(Error, TEXT("current weapon count is smalloer than 0"));
		return; 
	} 

	HoldWeapon(DummyGun);

}

void UWeaponActorComponent::MulticastSelectNum1_Implementation(FWeaponInfo SelectGunInfo)
{  
	SelectMainWeapon(SelectGunInfo);
}

void UWeaponActorComponent::MulticastSelectNum2_Implementation(FWeaponInfo SelectGunInfo)
{
	SelectSubWeapon(SelectGunInfo);
}

void UWeaponActorComponent::MulticastSelectNum3_Implementation(FWeaponInfo SelectGunInfo)
{
	SelectOtherWeapon(SelectGunInfo);
}

void UWeaponActorComponent::MulticastDestroyWeapon_Implementation(AK_YGGun* InWeaponToDestroy)
{
	K_YG_CHECK(InWeaponToDestroy);
	InWeaponToDestroy->Destroy();
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

 
 
void UWeaponActorComponent::EnterZoom()
{
	ServerEnterZoom();
}
 
void UWeaponActorComponent::ExitZoom()
{
	ServerExitZoom();
}
 
void UWeaponActorComponent::InputConvertDischarge()
{
	ServerConvertDischarge();
}


// step 1 - get weapon object from list that currently selected using 'GetCurrentSelectWeapon()'
// step 2 - spawn weapon actor in map and attach to player using 'HoldWeapon()'
void UWeaponActorComponent::SelectMainWeapon(FWeaponInfo SelectGunInfo)
{
	if (Character->IsLocallyControlled())
	{ 

		HoldWeapon(GetCurrentSelectWeapon());
		return;
	}
	HoldWeapon(GetWeapon(SelectGunInfo.WeaponKind));
}

void UWeaponActorComponent::SelectSubWeapon(FWeaponInfo SelectGunInfo)
{
	if (Character->IsLocallyControlled())
	{ 
		HoldWeapon(GetCurrentSelectWeapon());
		return;
	}
	HoldWeapon(GetWeapon(SelectGunInfo.WeaponKind));
}

void UWeaponActorComponent::SelectOtherWeapon(FWeaponInfo SelectGunInfo)
{
	if (Character->IsLocallyControlled())
	{
		HoldWeapon(GetCurrentSelectWeapon());
		return;
	}
	HoldWeapon(GetWeapon(SelectGunInfo.WeaponKind));
}

void UWeaponActorComponent::SelectNum1()
{

	if (Character->IsLocallyControlled())
	{
		SaveWeaponInfoInList(SpawnedGun->WeaponInfo);
	}

	GunInterface->SetWeaponImage(SelectWeaponIndex, SpawnedGun->WeaponInfo.WeaponKind, 0.5);
	SelectWeaponIndex = 0;
	ServerSelectNum1(GetCurrentSelectWeapon()->WeaponInfo);
}

void UWeaponActorComponent::SelectNum2()
{
	if (Character->IsLocallyControlled())
	{
		SaveWeaponInfoInList(SpawnedGun->WeaponInfo);
	}

	GunInterface->SetWeaponImage(SelectWeaponIndex, SpawnedGun->WeaponInfo.WeaponKind, 0.5);
	SelectWeaponIndex = 1;
	ServerSelectNum2(GetCurrentSelectWeapon()->WeaponInfo);
}

void UWeaponActorComponent::SelectNum3()
{
	if (Character->IsLocallyControlled())
	{
		SaveWeaponInfoInList(SpawnedGun->WeaponInfo);
	}

	GunInterface->SetWeaponImage(SelectWeaponIndex, SpawnedGun->WeaponInfo.WeaponKind, 0.5);
	SelectWeaponIndex = 2;
	ServerSelectNum3(GetCurrentSelectWeapon()->WeaponInfo);
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
	//// remove weapon in weapon list
	if (GetCurrentSelectWeapon()->WeaponInfo.WeaponKind == EWeapon::NONE)
	{
		K_YG_UELOG(Display, TEXT("can't drop weapon: already empty"));
		return;
	} 
	CurrentPlayerWeaponList[SelectWeaponIndex] = DummyGun;
	CurrentWeaponCount--;
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

AK_YGGun* UWeaponActorComponent::HoldWeapon(AK_YGGun* InWeaponToHold)
{
	if (SpawnedGun)
	{
		if (InWeaponToHold->WeaponInfo.WeaponKind == EWeapon::NONE)
		{
			// when player drop the weapon ( not change ) : throw weapon object on ground
			if (SpawnedGun->WeaponBody)
			{
				SpawnedGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				SpawnedGun->WeaponBody->SetSimulatePhysics(true);
				SpawnedGun->SetCollisionBlock();

				FVector Impulse = Character->GetActorForwardVector() * 300.f + FVector(0, 0, 200.f);
				SpawnedGun->WeaponBody->AddImpulse(Impulse, NAME_None, true);
			}
		} 
		else
		{
			// destroy weapon that player had ealry held 
			// just change weapon  
			SpawnedGun->Destroy();
		}
	}

	//AK_YGGun* NewWeapon = NewObject<AK_YGGun>();
	if (!InWeaponToHold)
	{
		K_YG_UELOG(Error, TEXT("Weapon To hold is null ptr"));
		return nullptr;
	}
	EWeapon WeaponKind = InWeaponToHold->WeaponInfo.WeaponKind;
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
		// initiate spawned gun
		SpawnedGun->SetOwnerCharacter(Character);
		SpawnedGun->InitCharacterInstance();
		SpawnedGun->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		// set bullet fire location: get from weapon socket's location
		SpawnedGun->SetBulletFireLocation(SKMesh->GetSocketLocation(WeaponSocket));
		//K_YG_UELOG(Error, TEXT("Test Gun Create "));
		// get original weapon info from inventory   
		SpawnedGun->SetGunInterfaceWidget(GunInterface);

		// set collision overlap to prevent player character move weird
		SpawnedGun->SetCollisionOverlap();
		SpawnedGun->SetCameraComponent(GetCameraComponent());
		SpawnedGun->WeaponInfo = InWeaponToHold->WeaponInfo;
		SpawnedGun->UpdateBulletWidgetText(); 

		// change widget iamge
		GunInterface->SetWeaponImage(SelectWeaponIndex, InWeaponToHold->WeaponInfo.WeaponKind, 1.0);
		//K_YG_SIMPLE(Display);

		Cast<AK_YGOnlyFightCharacter>(Character)->SetWeaponState(WeaponKind);
		//SetAnimWeaponState(WeaponKind);

	}


	return SpawnedGun;
}
 


// when player character killed, drop all item and init HP
void UWeaponActorComponent::DropAllWeapon()
{
	for (int curIdx = SelectWeaponIndex; curIdx < MaxWeaponListSize; curIdx++)
	{
		DropWeapon();
	}

}
