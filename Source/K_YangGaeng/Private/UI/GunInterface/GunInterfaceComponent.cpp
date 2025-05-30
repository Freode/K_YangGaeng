// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/GunInterface/GunInterfaceComponent.h"
#include "UI/GunInterface/K_YGGunInterfaceWidget.h"
#include "Weapon/K_YGGun.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UGunInterfaceComponent::UGunInterfaceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// load blueprint widget class
	static ConstructorHelpers::FClassFinder<UUserWidget> GunInterFaceObj(TEXT("/Game/K_YangGaeng/UI/GunInterface/WG_GunInterface.WG_GunInterface_C"));
	if (GunInterFaceObj.Succeeded())
	{
		GunInterfaceWidgetClass = GunInterFaceObj.Class;
	}

	// set bullet value (modify after: get value from weapon information)
	TotalBullet = 100;
	LoadedBullet = 50;
	InitializeComponents();
	// ...
}

bool UGunInterfaceComponent::InitializeComponents()
{
	HandGunTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/K_YangGaeng/UI/ShopSystem/HandGun.HandGun"));
	RifleTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/K_YangGaeng/UI/ShopSystem/Riple1.Riple1"));
	SniperRifleTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/K_YangGaeng/UI/ShopSystem/Sniper.Sniper"));
	ShotGunTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/K_YangGaeng/UI/ShopSystem/ShotGun1.ShotGun1"));
	SMGTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/K_YangGaeng/UI/ShopSystem/SMG.SMG"));

	if (CheckTextureIsValid())
	{
		return true;
	}

	K_YG_UELOG(Display, TEXT("Initialize Comonent failed"));
	return false;
}

bool UGunInterfaceComponent::CheckTextureIsValid()
{
	if (RifleTexture && SniperRifleTexture && ShotGunTexture && SMGTexture)
	{
		return true;
	}
	K_YG_UELOG(Display, TEXT("Texture is not valid"));
	return false;
}


void UGunInterfaceComponent::SetWeaponImage(int idx, EWeapon WeaponKind, float InAlphaValue)
{
	if (!Character->IsLocallyControlled())
	{
		return;
	}
	if (!CheckTextureIsValid())
	{ 
		return;
	}
	switch (WeaponKind)
	{
	case EWeapon::NONE:
		GunInterfaceWidget->ChangeImage(idx, nullptr, 0.0);
		break;
	case EWeapon::HANDGUN:
		GunInterfaceWidget->ChangeImage(idx, HandGunTexture, InAlphaValue);
		break;
	case EWeapon::RIFLE:
		GunInterfaceWidget->ChangeImage(idx, RifleTexture, InAlphaValue);
		break;
	case EWeapon::SNIPERRIFLE:
		GunInterfaceWidget->ChangeImage(idx, SniperRifleTexture, InAlphaValue);
		break;
	case EWeapon::SMG:
		GunInterfaceWidget->ChangeImage(idx, SMGTexture, InAlphaValue);
		break;
	case EWeapon::SHOTGUN:
		GunInterfaceWidget->ChangeImage(idx, ShotGunTexture, InAlphaValue);
		break;
	}
}
 

// Called when the game starts
void UGunInterfaceComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		K_YG_UELOG(Error, TEXT("Character is nullptr"));
	}

	if (GunInterfaceWidgetClass && Character->IsLocallyControlled())
	{
		GunInterfaceWidget = CreateWidget<UK_YGGunInterfaceWidget>(GetWorld(), GunInterfaceWidgetClass);
		if (GunInterfaceWidget)
		{
			GunInterfaceWidget->AddToViewport();
			GunInterfaceWidget->SetTotalBulletText(TotalBullet);
			GunInterfaceWidget->SetLoadedBulletText(LoadedBullet);
			InitializeComponents();
		}
		else
		{
			K_YG_UELOG(Display, TEXT("widget failed"));
		}
	} 

	// ...
	
}

void UGunInterfaceComponent::InitInterface()
{
}
 


// Called every frame
void UGunInterfaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunInterfaceComponent::UpdateBulletText(int NewLoadedBulletText, int NewTotalBulletText)
{
	GunInterfaceWidget->SetTotalBulletText(NewTotalBulletText);
	GunInterfaceWidget->SetLoadedBulletText(NewLoadedBulletText);
} 

