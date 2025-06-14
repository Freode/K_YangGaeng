// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGGun.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/K_YGObjectPool.h"
#include "Weapon/K_YGTestOnlyFightCharacter.h"
#include "Character/CameraActorComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UI/GunInterface/GunInterfaceComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Character/BaseCharacterAnimInstance.h"


class AK_YGBulletBase;

AK_YGGun::AK_YGGun()
{ 

	// Setup UI

	// Find the widget blueprint class and set it
	static ConstructorHelpers::FClassFinder<UUserWidget> IronsightWidgetClassFinder(TEXT("/Game/K_YangGaeng/Weapons/WeaponUI/WB_IronsightCrosshair.WB_IronsightCrosshair_C"));
	if (IronsightWidgetClassFinder.Succeeded())
	{
		IronsightCrosshairWidgetClass = IronsightWidgetClassFinder.Class;
	}


	// Find the widget blueprint class and set it
	static ConstructorHelpers::FClassFinder<UUserWidget> SniperWidgetClassFinder(TEXT("/Game/K_YangGaeng/Weapons/WeaponUI/WB_SniperCrosshair.WB_SniperCrosshair_C"));
	if (SniperWidgetClassFinder.Succeeded())
	{
		SniperCrosshairWidgetClass = SniperWidgetClassFinder.Class;
	}
	WeaponInfo.WeaponKind = EWeapon::NONE;
	CheckPlayerCharacter();

	WeaponInfo.BulletErrorRange = MaxBulletErrorRange;
	
}


void AK_YGGun::BeginPlay()
{
	Super::BeginPlay();
	// Bullet pool init
	BulletPool = NewObject<UK_YGObjectPool>();
	BulletClass = AK_YGBulletBase::StaticClass();
	BulletPool->InitializePool(GetWorld(), BulletClass);


	if (ForwardArrow)
	{
		ForwardArrow->SetWorldRotation(GetActorForwardVector().Rotation());
	}
	//DrawArrow();	
	
	//if (Character == nullptr)
	//{
	//	K_YG_UELOG(Error, TEXT("Character is nullptr"));
	//}


	// init ironsight crosshair widget widgets
	IronsightCrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), IronsightCrosshairWidgetClass);
	IronsightCrosshairWidget->AddToViewport();
	IronsightCrosshairWidget->SetVisibility(ESlateVisibility::Visible);

	// init sniper crosshair widget
	SniperCrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), SniperCrosshairWidgetClass);
	SniperCrosshairWidget->AddToViewport();
	SniperCrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
	  
	WeaponInfo.BulletErrorRange = MaxBulletErrorRange;
}

void AK_YGGun::CheckPlayerCharacter()
{
	if (nullptr == Character)
	{
		Character = Cast<ACharacter>(GetOwner());
	}

}

// fira ray 
void AK_YGGun::FireRay()
{
	FVector StartLocation;
	FRotator StartRotation;

	// find camera's location and rotation
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(StartLocation, StartRotation);
	}
	 
	
	// if ray hit nothing, fire to ray's end location
	FVector EndLocation = StartLocation + (StartRotation.Vector() * RayLength);

	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = Character->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		CollisionParams
	);

	if (bHit)
	{ 
		AimRayCollisionLocation = HitResult.Location;
	} 
	else
	{ 
		AimRayCollisionLocation = StartLocation + (StartRotation.Vector() * RayLength);
	}
	//DrawDebugLine(GetWorld(), StartLocation, AimRayCollisionLocation, FColor::Green, false, 1, 0,3);
	//DrawDebugPoint(GetWorld(), AimRayCollisionLocation, 10, FColor::Red, false, 1);

	//APlayerState* PS = Character->GetPlayerState<APlayerState>();
	//K_YG_UELOG(Error, TEXT("%d: AimRayCollisionLocation is %s"), PS->GetPlayerId(), *AimRayCollisionLocation.ToString());
}

bool AK_YGGun::CheckHoveringWeapon()
{ 
	if (!CameraComponent)
	{
		K_YG_UELOG(Error, TEXT("Camera Component is nullptr"));
		return false;
	}
	return CameraComponent->bIsHovered;
}

AK_YGGun* AK_YGGun::PickHoveredWeapon()
{
	AK_YGGun* HoveredWeapon = Cast<AK_YGGun>(CameraComponent->GetHoveredWeaponClass());	
	if (!HoveredWeapon)
	{
		K_YG_UELOG(Error, TEXT("Hovered Weapon is nullptr"));
		HoveredWeapon = NewObject<AK_YGGun>();
	}
	return HoveredWeapon;
}

void AK_YGGun::ReturnMontageNull()
{	
	// Upper body idle animation asset
	if (UpperBodyOriginalIdle)
	{
		UpperBodyIdle = UpperBodyOriginalIdle;
	}
}

void AK_YGGun::UpdateBulletWidgetText()
{
	if (GunInterface && Character->IsLocallyControlled())
	{ 
		GunInterface->UpdateBulletText(WeaponInfo.CurrentUsableBulletCount, WeaponInfo.MaxUsableBulletCount);
	} 
}

FVector AK_YGGun::CalculateBulletDirection(FVector InAimPosition)
{
	// create random bullet position caculate with BulletErrorRange
	float NewAimPositionX = InAimPosition.X + FMath::FRandRange(-1 * WeaponInfo.BulletErrorRange, WeaponInfo.BulletErrorRange);
	float NewAimPositionY = InAimPosition.Y + FMath::FRandRange(-1 * WeaponInfo.BulletErrorRange, WeaponInfo.BulletErrorRange);
	float NewAimPositionZ = InAimPosition.Z + FMath::FRandRange(-1 * WeaponInfo.BulletErrorRange, WeaponInfo.BulletErrorRange);
	FVector NewBulletVector(NewAimPositionX, NewAimPositionY, NewAimPositionZ);

	return NewBulletVector;
}

void AK_YGGun::Tick(float DeltaTime)
{
	// check if next bullet is ready
	if (bIsDischarged == false) 
	{
		WaitNextBulletCharge(DeltaTime);
	}

	// check if player is reloading
	if (bIsReloading) 
	{
		WaitReload(DeltaTime);
	}

	// check if player is already firing
	if (bIsFiring)
	{
		Fire();
	}

	// check if player pressed mouse left button(start aim)
	if (bIsAimStarted)
	{
		Aiming(DeltaTime);
	}
	 
	Super::Tick(DeltaTime);
}



// when bulle is fired repeatedly, wait untill next bullet will charge
void AK_YGGun:: WaitNextBulletCharge(float DeltaTime) {
	CurrentDischargeTime += DeltaTime;
	//K_YG_UELOG(Warning, TEXT("Wait Bullet Charged: %f"), DeltaTime);

	if (CurrentDischargeTime > WeaponInfo.NextBulletDischargeTime) {
		CurrentDischargeTime = 0.0f;
		bIsDischarged = true;
	}
}


void AK_YGGun::ConvertDisChargeModeAll()
{
	if (bIsAbleBurstShot) 
	{
		bIsAbleBurstShot = false;
		K_YG_UELOG(Warning, TEXT("Converto Burst to Full auto"));
	}
	else
	{
		bIsAbleBurstShot = true;
		K_YG_UELOG(Warning, TEXT("Converto Repeat to Burst"));
	}
}



void AK_YGGun::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{ 
	if (!bInterrupted)
	{
		//UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		UBaseCharacterAnimInstance* BaseAnimInstance= Cast<UBaseCharacterAnimInstance>(CharacterAnimInstance);
		BaseAnimInstance->SetAnimWeaponState(WeaponInfo.WeaponKind);
		CharacterAnimInstance->Montage_Stop(0.2f, UpperBodyReload);
		K_YG_SIMPLE(Display); 
		
	}

}

void AK_YGGun::InitCharacterInstance()
{
	// init character anim instance
	CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();

	// check if instance is null
	if (CharacterAnimInstance == nullptr)
	{
		K_YG_UELOG(Error, TEXT("Character Anim Instance is nullptr"));
		return;
	}

	// Montage Animation Play
	if (UpperBodyIdle == nullptr)
	{
		K_YG_UELOG(Error, TEXT("UpperBodyIdle montage is nullptr"));
		return;
	}
	// Play Idle anim montage
	//CharacterAnimInstance->Montage_Play(UpperBodyIdle, 0.0f);
	

}

void AK_YGGun::ChangeZoomMode(EZoomMagnificationMode ZoomMode)
{
	K_YG_UELOG(Error, TEXT("Change Zoom Mode"));
	switch (ZoomMode)
	{
	case EZoomMagnificationMode::IRONSIGHT:
		WeaponInfo.BulletErrorRange = 10.0f;
		CameraComponent->SpringArmComponent->TargetArmLength = 200;
		break;
	case EZoomMagnificationMode::X1:
		WeaponInfo.BulletErrorRange = 5.0f;
		CameraComponent->SpringArmComponent->SetRelativeLocation(FVector(0.0f, 30.0f, 60.0f));
		CameraComponent->SpringArmComponent->TargetArmLength = -50.0;
		CameraComponent->SpringArmComponent->SetRelativeRotation(FRotator::ZeroRotator);

	case EZoomMagnificationMode::X2:

		break;
	case EZoomMagnificationMode::X4:

		break;
	case EZoomMagnificationMode::X6:

		break;
	}
}



void AK_YGGun::InitMontage()
{
}




void AK_YGGun::CreateBullet()
{
	if (Character->HasAuthority()) {
		BulletFireLocation = Character->GetMesh()->GetSocketLocation(TEXT("hand_rSocket_HandGun"));
		FireRay(); 
		//K_YG_UELOG(Display, TEXT("Bullet fire location: %s"), *BulletFireLocation.ToString());
		// fire towward ray occured collision
		FVector SpawnRotation = (CalculateBulletDirection(AimRayCollisionLocation) - BulletFireLocation).GetSafeNormal();
		// Get Bullet Object from pool class
		AK_YGBulletBase* Bullet = BulletPool->GetBullet();
		if (Bullet)
		{

			// Caculate muzzle rotator vector and fire bullet 

			// set bullet offset and fire
			Bullet->SetActorLocation(BulletFireLocation);
			//K_YG_UELOG(Display, TEXT("Bullet spawnrotation: %s"), *SpawnRotation.ToString());
			//K_YG_UELOG(Display, TEXT("Bullet error range is %f"), WeaponInfo.BulletErrorRange);
			//Bullet->SetActorRotation(SpawnRotation);
			Bullet->Activate();
			Bullet->SetBulletDamage(10);
			//K_YG_UELOG(Display, TEXT("Bullet Damage: %d"), WeaponInfo.WeaponDamage);
			Bullet->FireInDirection(WeaponInfo.BulletVelocity, SpawnRotation);
			Bullet->SetCharacter(Character);
		}
		else
		{ 
			// if queue is empty ( bullet object in queue is not enough)
			// then increase PoolSize 
			K_YG_UELOG(Display, TEXT("Bullet pool queue is empty"));
			BulletPool->InitializePool(GetWorld(), BulletClass);
		}
	} 

}



void AK_YGGun::Fire() 
{ 

	if (WeaponInfo.CurrentUsableBulletCount >= 1
		&& bIsDischarged && bIsReloading == false) {
		CreateBullet();
		switch (WeaponInfo.DischargeMode)
		{

		case EWeaponDischargeMode::SINGLESHOT:
			WeaponInfo.CurrentUsableBulletCount -= 1;
			//K_YG_UELOG(Warning, TEXT("Testing Single Shot"));

			//연발 하지 못하도록 한 번 발사하면 강제로 ExitFire 호출 
			ExitFire();      
			break;

		case EWeaponDischargeMode::BOLTACTION:
			WeaponInfo.CurrentUsableBulletCount -= 1;
			//K_YG_UELOG(Warning, TEXT("Testing Bolt Action"));
			// if they Aiming, exit aiming and end fire to reload
			if (bIsZoomMagnification) 
			{
				ExitAim();
			}
			ExitFire();
			break;

		case EWeaponDischargeMode::ALL:

			// 연발 선택
			if (bIsAbleBurstShot == false)
			{
				WeaponInfo.CurrentUsableBulletCount -= 1;
				//K_YG_UELOG(Warning, TEXT("Testing Full Auto"));
				break;
			} 
			// Burst 모드를 선택한 경우 다음 case로 넘어감
		case EWeaponDischargeMode::BURST:
			CurrentBurstShot += 1;
			WeaponInfo.CurrentUsableBulletCount -= 1;
			//K_YG_UELOG(Warning, TEXT("Testing Burst"));
			// 3 발 다 쏘고 난 후
			if (CurrentBurstShot == 3) 
			{
				// 다시 초기화 
				CurrentBurstShot = 0; 
				// 다음 발사 시간 원래대로
				WeaponInfo.NextBulletDischargeTime *= 2;
				bIsFiring = false;
			}
			// 3점사 중 첫 번째 총알
			else if (CurrentBurstShot == 1) 
			{
				// 빠르게 연사해야 하니 연사속도 1/2
				WeaponInfo.NextBulletDischargeTime /= 2; 
			}
			break;

			// 연발이므로 ExitFire() 없이 누르고 있는 만큼 계속 발사
		case EWeaponDischargeMode::FULLAUTO:      
			WeaponInfo.CurrentUsableBulletCount -= 1;
			//K_YG_UELOG(Warning, TEXT("Testing Full Auto"));
			break;

		default:
			break;
		}
		// after this variable set 'false', WaitNextBulletCharged() is called and turn to 'true'
		bIsDischarged = false;
		UpdateBulletWidgetText();

		// auto reload when loaded bullet remain 0
		if (WeaponInfo.CurrentUsableBulletCount <= 0)
		{
			EnterReload();
		}
	}

	return;


}

void AK_YGGun::EnterFire()
{
	bIsFiring = true;

	Cast<UBaseCharacterAnimInstance>(CharacterAnimInstance)->SetIsPlayerAnimOn(bIsFiring);
	// Montage Animation Play

	if (UpperBodyFire == nullptr)
	{
		K_YG_UELOG(Error, TEXT("UpperBodyFire montage is nullptr"));
		return;
	} 
	if (CharacterAnimInstance == nullptr)
	{
		K_YG_UELOG(Error, TEXT("Character Anim Instance is nullptr"));
		return;
	}


}

// 이 함수가 실행되면 총알 발사가 종료된다.
void AK_YGGun::ExitFire()  
{
	// Burst 형식인 총기류의 경우 마우스를 떼도 ExitFire 하지 않음
	// 3 발이 다 나갈 때 까지  
	if (WeaponInfo.DischargeMode == EWeaponDischargeMode::BURST ||
		// 발사 모드가 Burst 인 경우, 마우스를 떼도 ExitFire 하지 않도록(잠깐 눌러도 세 발이 나가도록)
		WeaponInfo.DischargeMode == EWeaponDischargeMode::ALL && bIsAbleBurstShot)  
	{
		return;
	}
	bIsFiring = false;

	Cast<UBaseCharacterAnimInstance>(CharacterAnimInstance)->SetIsPlayerAnimOn(bIsFiring);
}


// start reloading and change bullet size
void AK_YGGun::EnterReload()
{
	K_YG_UELOG(Warning, TEXT("Reload..."));
	if (WeaponInfo.MaxUsableBulletCount >= WeaponInfo.MagazinBulletCount
		&& WeaponInfo.CurrentTotalBulletCount == 0)
	{
		WeaponInfo.MaxUsableBulletCount -= WeaponInfo.MagazinBulletCount;
		WeaponInfo.CurrentUsableBulletCount = WeaponInfo.MagazinBulletCount;
		bIsReloading = true;
		//Cast<UBaseCharacterAnimInstance>(CharacterAnimInstance)->SetIsPlayerReloadOn(bIsReloading);


		if (!UpperBodyReload)
		{
			K_YG_UELOG(Error, TEXT("reload montage is nullptr"));
		}
		 
		// Play parkour anim montage
		CharacterAnimInstance->Montage_Play(UpperBodyReload, 1.0f);  

		//CharacterAnimInstance->Montage_Stop(0.2f, UpperBodyReload);
		MontageEndedDelegate.BindUObject(this, &AK_YGGun::OnMontageEnded);
		CharacterAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, UpperBodyReload);
	}
	else
	{
		K_YG_UELOG(Display, TEXT("reload failed"));
	}

	UpdateBulletWidgetText();

	EWeaponToString(this->WeaponInfo.WeaponKind);
 
}



// After EnterReload(), wait untill reload is done
void AK_YGGun::WaitReload(float DeltaTime)
{
	CurrentReloadingTime += DeltaTime;
	
	// if they have enough bullet to reload
	if (CurrentReloadingTime >= WeaponInfo.ReloadTime) 
	{
		bIsReloading = false;
		//Cast<UBaseCharacterAnimInstance>(CharacterAnimInstance)->SetIsPlayerReloadOn(bIsReloading);
		CurrentReloadingTime = 0.0f;
		K_YG_UELOG(Warning, TEXT("Complete Reload - Current Bullet: %d"), WeaponInfo.CurrentUsableBulletCount);
	}  


	 
}

// 조준상태 진입
void AK_YGGun::EnterAim()
{	
	bIsAimStarted = true;
	//ChangeZoomMode(EZoomMagnificationMode::IRONSIGHT);

}

// 조준상태 탈출
void AK_YGGun::ExitAim()   
{
	K_YG_SIMPLE(Display);
	CameraComponent->ChangeViewMode(EViewMode::THIRD_PERSON);
	// if pressing time is long enugh, then exit aim and enter zoom magnificatoin
	if (CurrentZoomDelay > MaxZoomDelay)
	{
		CurrentZoomDelay = 0.0f;
		// montage play: exit aim and return to idle montage
		CheckPlayerCharacter();

		if (Character == nullptr)
		{
			K_YG_UELOG(Error, TEXT("Character is nullptr"));
		}
		CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
		// Play parkour anim montage
		//CharacterAnimInstance->Montage_Play(UpperBodyIdle, 0.0f);
		//MontageEndedDelegate.BindUObject(this, &AK_YGGun::OnMontageEnded);
		//CharacterAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, UpperBodyIdle);


		return;
	}
	K_YG_UELOG(Warning, TEXT("Exit Zoom..."));
	CurrentZoomDelay = 0.0f;
	bIsAimStarted = false;
	//Cast<UBaseCharacterAnimInstance>(CharacterAnimInstance)->SetIsPlayerReloadOn(bIsAimStarted);
	Zooming();

}

void AK_YGGun::Zooming()
{		
	CheckPlayerCharacter();
	// montage play
	if (Character == nullptr)
	{
		K_YG_UELOG(Error, TEXT("Character is nullptr"));
	}


	K_YG_UELOG(Warning, TEXT("Enter Zoom..."));
	// if player click mouse right button when zoom magnification is already excutign,
	// then exit zoom
	if (bIsZoomMagnification)
	{
		if (CameraComponent->SceneCaptureComponent && CameraComponent->RenderTarget && SniperCrosshairWidget)
		{
			// Hide CrosshairWidget UI	if (SniperCrosshairWidget)
			SniperCrosshairWidget->SetVisibility(ESlateVisibility::Hidden);

			// set visible is true when zoom is end
			Character->GetMesh()->SetVisibility(true);
			WeaponBody->SetVisibility(true);
			CameraComponent->SceneCaptureComponent->bCaptureEveryFrame = true;
		}
		bIsZoomMagnification = false;
		
		// return to nomal view mode
		CameraComponent->ChangeViewMode(EViewMode::THIRD_PERSON);

		// init bullet error range to normal
		WeaponInfo.BulletErrorRange = MaxBulletErrorRange;
		 
		return;
	}
	bIsZoomMagnification = true;

	// Show CrosshairWidget UI 
	if (CameraComponent->SceneCaptureComponent && CameraComponent->RenderTarget && SniperCrosshairWidget && Character->IsLocallyControlled())
	{
		SniperCrosshairWidget->SetVisibility(ESlateVisibility::Visible);

		// hide meshes when player is zoom
		Character->GetMesh()->SetVisibility(false);
		WeaponBody->SetVisibility(false);

		CameraComponent->SceneCaptureComponent->bCaptureEveryFrame = false;
	}
	// enter to weapon's zoom magnificaton zoom
	ChangeZoomMode(WeaponInfo.ZoomMagnificationMode); 
	CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();  
}

void AK_YGGun::Aiming(float DeltaTime)
{
	CurrentZoomDelay += DeltaTime;


	if (CurrentZoomDelay > MaxZoomDelay)
	{
		bIsAimStarted = false;
	}
	 
	 

}



