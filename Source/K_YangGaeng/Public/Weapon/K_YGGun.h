// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/K_YGBulletBase.h"
#include "Weapon/K_YGObjectPool.h" 
#include "K_YGGun.generated.h"

/**
 * 
 */

class ArrowComponent;
class UCameraActorComponent;
class UGunInterfaceComponent;

UENUM(BlueprintType)
enum class EZoomMagnificationMode : uint8
{
	// 없음
	IRONSIGHT,
	// 허리에 차는 방식
	HIP,
	// 1배율
	X1,
	// 2배율
	X2,
	// 4배율
	X4,
	// 6배율
	X6,
};

UENUM(BlueprintType)
enum class EWeaponDischargeMode : uint8
{
	// 단발 / 연속 사격 가능
	SINGLESHOT,

	// 단발 / 연속 사격 불가능
	BOLTACTION,

	// 연발 x / 3점사 o
	BURST,

	// 연발 o / 3점사 x
	FULLAUTO,

	// 연발 o / 3점사 o
	ALL
};

USTRUCT(BlueprintType)
struct FWeaponInfo
{ 
	GENERATED_BODY();

	// 총기 종류
	UPROPERTY()
	EWeapon WeaponKind;

	// 총기 데미지
	UPROPERTY()
	float WeaponDamage;

	// 탄창 최대 장전 가능한 총알 개수
	UPROPERTY()
	int16 MagazinBulletCount;

	// 현재 장전된 총알 개수
	UPROPERTY()
	int16 CurrentUsableBulletCount;

	// 최대 소지 총알 개수
	UPROPERTY()
	int16 MaxUsableBulletCount;

	// 현재 소지한 총알 전체 개수
	UPROPERTY()
	int16 CurrentTotalBulletCount;

	// 총기 반동 계수
	UPROPERTY()
	float ReboundFoffeicient;

	// 총기 발사 모드
	UPROPERTY()
	EWeaponDischargeMode DischargeMode;

	// 장전 속도
	UPROPERTY()
	float ReloadTime;

	// 캐릭터 이동속도 보정(무게로 인한)
	UPROPERTY()
	float CharacterSpeedRevicseWeight;

	// 총기 연사 시간
	UPROPERTY()
	float NextBulletDischargeTime;
	
	// 총알 발사 속도
	UPROPERTY()
	float BulletVelocity;

	// 줌과 확대경 비율
	UPROPERTY()
	EZoomMagnificationMode ZoomMagnificationMode;

	// 총알 발사 위치 정확도(오차계수): 줌 모드에 따라 달라짐 
	UPROPERTY()
	float BulletErrorRange;

};



UCLASS()
class K_YANGGAENG_API AK_YGGun : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AK_YGGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	void CheckPlayerCharacter();


	// get skeletal mesh from character component 
	// and init montage object (idle, fire, reload) differently depending on weapon kind
	virtual void InitMontage();


	// Sniper Crosshair Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SniperCrosshairWidgetClass;


	// All ironsight Crosshair Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> IronsightCrosshairWidgetClass;

	// Character Gun Interface widget
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGunInterfaceComponent> GunInterface;

	// max bullet error range: when player don't activate zoom
	float MaxBulletErrorRange = 20.0f;

	// caculate bullet fire direction consider with error range
	FVector CalculateBulletDirection(FVector InAimPosition);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FVector AimRayCollisionLocation;

	void SetAimRayCollisionLocation(FVector AimCurrentLocation) { AimRayCollisionLocation = AimCurrentLocation; }
	FVector GetAimRayCollisionLocation() { return AimRayCollisionLocation; }
	 

	// Fire System

	// Actually Fire bullet
	void Fire();

	// to fire bullet repeatidly
	void EnterFire();
	void ExitFire();

	// Create bullet object and spawn in game to move
	void CreateBullet();

	void EnterReload();
	void WaitReload(float DeltaTime);

	// Aiming System
	void EnterAim();
	void Zooming();
	void Aiming(float DeltaTime);
	void ExitAim();

	void WaitNextBulletCharge(float DeltaTime);

	// if DischargeMode is 'ALL', then can convert between 'FULLAUTO' and 'BURST'
	void ConvertDisChargeModeAll();

	// match timing with character class
	//void SetupCameraComponent();
	void SetCameraComponent(UCameraActorComponent* CameraActorComponent) { CameraComponent = CameraActorComponent; }
	UCameraActorComponent* GetCameraComponent() { return CameraComponent; }

	// after special montage end, then return to idle montage
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// init character instance after actor is really spawned in game
	void InitCharacterInstance();

	// change zoom mode : change camera angel and length
	void ChangeZoomMode(EZoomMagnificationMode ZoomMode);

	void SetOwnerCharacter(ACharacter* OwnerCharacter) { Character = OwnerCharacter; }

	// fire ray and check collision location
	void FireRay();

	// check if player camera is hovering weapon with crosshair
	bool CheckHoveringWeapon();

	AK_YGGun* PickHoveredWeapon();


	FWeaponInfo WeaponInfo;

	// return montage to nothing to hold(null)
	void ReturnMontageNull();


	// bullet widget text
	void SetGunInterfaceWidget(UGunInterfaceComponent* NewGunInterface) { GunInterface = NewGunInterface; }
	void UpdateBulletWidgetText();

	// Sniper Crosshair UI
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UUserWidget* SniperCrosshairWidget;

	// All ironsight crosshair UI
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UUserWidget* IronsightCrosshairWidget;

	// set bullet position : get from weapon socket's location
	void SetBulletFireLocation(FVector InBulletFirePosition) { BulletFireLocation = InBulletFirePosition; }

private:
	bool bIsAimStarted = false;
	bool bIsZoomMagnification = false;
	bool bIsReloading = false;
	bool bIsDischarged = true;

	// 발사 하고 있는 동안 true(연발인 경우 계속해서 Fire() 함수 실행할 필요 있으므로)
	bool bIsFiring = false;			

	// 발사 방식이 ALL인 경우 연발/3점사 중 어떤게 선택되었는지(true인 경우 3점사 선택)
	bool bIsAbleBurstShot = false;		

	bool bIsBurstDischarged = false;	

	
	// Busrt 모드에서 3발 내 다음 총알 속도
	float BurstDischargedTime = 0.05f;
	// 일반 총알 속도
	float CurrentDischargeTime = 0.0f;
	float CurrentReloadingTime = 0.0f;
	float CurrentShootErrorRange = 1.0f;
	float MaxShootErrorRange = 1.0f;

	// check time how long mouse right button has been pressed, 
	// and decide which will be executed(enter zoom or just aiming)
	float MaxZoomDelay = 0.2f;
	float CurrentZoomDelay = 0.0f;

	// count burst shot bullet until it reach 3
	int CurrentBurstShot = 0;

	// ray
	UPROPERTY(EditAnywhere)
	float RayLength = 100000.0f;  


	// Object pooling

	// Bullet Pool
	UPROPERTY()
	UK_YGObjectPool* BulletPool;

	// Bullet class
	UPROPERTY()
	TSubclassOf<AK_YGBulletBase> BulletClass;

	// Pool Size
	UPROPERTY(EditAnywhere)
	int32 PoolSize;

	// Fire location
	UPROPERTY(EditAnywhere)
	FVector SpawnOffset = FVector::ForwardVector;

	// Debug
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ForwardArrow;

	UPROPERTY()
	UCameraActorComponent* CameraComponent;

	FVector BulletFireLocation;
	 

	 




};
