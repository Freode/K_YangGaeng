// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "WeaponActorComponent.generated.h"

class AK_YGTestOnlyFightCharacter;
class AK_YGGun;
class UOnlyFightInputAComponent;
class UCameraActorComponent;
class UGunInterfaceComponent;
enum class EWeapon : uint8;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UWeaponActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponActorComponent();

	// Gun Function 
	UFUNCTION()
	void InputStartFire();

	UFUNCTION()
	void InputStopFire();

	UFUNCTION()
	void InputReload();

	UFUNCTION()
	void EnterZoom();

	UFUNCTION()
	void ExitZoom();

	UFUNCTION()
	void InputConvertDischarge();

	UFUNCTION()
	void SelectMainWeapon(FWeaponInfo SelectGunInfo);

	UFUNCTION()
	void SelectSubWeapon(FWeaponInfo SelectGunInfo);

	UFUNCTION()
	void SelectOtherWeapon(FWeaponInfo SelectGunInfo);

	// press key '1'
	void SelectNum1();

	// press key '2'
	void SelectNum2();

	// press key '3'
	void SelectNum3();


	void SetGunInterfaceWidget(UGunInterfaceComponent* NewGunInterface) { GunInterface = NewGunInterface; }

	// stuff weapon that player picked in weaopn list(inventory)
	bool StuffWeaponIsList(EWeapon WeaponKind);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// save weaopn in list after use it
	void SaveWeaponInfoInList(FWeaponInfo InWeaponInfo);

	UPROPERTY()
	ACharacter* Character;

	// weapon that player is holding
	UPROPERTY()
	AK_YGGun* SpawnedGun;

	UPROPERTY()
	UCameraActorComponent* CameraComponent;

	// Character Gun Interface widget
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGunInterfaceComponent> GunInterface;


	// ---------- server function

	void ServerInputStartFire_Implementation();

	void ServerInputStopFire_Implementation();

	void ServerInputReload_Implementation();

	void ServerEnterZoom_Implementation();

	void ServerExitZoom_Implementation();

	void ServerConvertDischarge_Implementation();

	void ServerGiveDamage_Implementation(float Damage);

	void ServerUpdateCurrentWeapon_Implementation();

	void ServerPickWeapon_Implementation(EWeapon InteractWeaponKind);

	void ServerDropWeapon_Implementation();

	void ServerSelectNum1_Implementation(FWeaponInfo SelectGunInfo);

	void ServerSelectNum2_Implementation(FWeaponInfo SelectGunInfo);

	void ServerSelectNum3_Implementation(FWeaponInfo SelectGunInfo);

	void ServerDestroyWeapon_Implementation(AK_YGGun* InWeaponToDestroy);
 


	void MulticastInputStartFire_Implementation();

	void MulticastInputStopFire_Implementation();

	void MulticastInputReload_Implementation();

	void MulticastEnterZoom_Implementation();

	void MulticastExitZoom_Implementation();

	void MulticastConvertDischarge_Implementation();

	void MulticastGiveDamage_Implementation(float Damage);

	void MulticastUpdateCurrentWeapon_Implementation();

	void MulticastPickWeapon_Implementation(EWeapon InteractWeaponKind);

	void MulticastDropWeapon_Implementation();

	void MulticastSelectNum1_Implementation(FWeaponInfo SelectGunInfo);

	void MulticastSelectNum2_Implementation(FWeaponInfo SelectGunInfo);

	void MulticastSelectNum3_Implementation(FWeaponInfo SelectGunInfo);

	void MulticastDestroyWeapon_Implementation(AK_YGGun* InWeaponToDestroy);


 
	//skeletal mesh
	UPROPERTY()
	USkeletalMeshComponent* SKMesh;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// debuging log function - print EWeapon to String
	void EWeaponToString(EWeapon WeaponKind);


	// Create weapon object
	// use this function when they pick item in map and stuff it in inventory(weapon list) 
	UFUNCTION()
	AK_YGGun* GetWeapon(EWeapon WeaponKind);

	// actually spawn weapon actor in map so player can hold it
	UFUNCTION()
	AK_YGGun* HoldWeapon(AK_YGGun* InWeaponToHold);
	 
	// check if player character is available
	void CheckPlayerCharacter();

	// bind input
	void BindInput(UOnlyFightInputAComponent* InputCustomComponent);


	// Init player's weapon lists
	void InitItemList();
	 
	// Drop weapon that current holding
	void DropWeapon();
	 

	// synchronize weapon info before player select other weapon(and put currently holding weapon in weapon list)
	// call this function when player press '1' or '2'
	// might not need 'cause they share same address, but just in case
	void UpdateWeaponInfo(int16 CurrentWeaponListIndex);

	// get currently selected item in inventory
	UFUNCTION()
	AK_YGGun* GetCurrentSelectWeapon() { return CurrentPlayerWeaponList[SelectWeaponIndex]; }

	void SetSpawnedGun(AK_YGGun* InGun) { SpawnedGun = InGun; }

	// set and get camera component
	void SetCameraComponent(UCameraActorComponent* InCameraActorComponent) { CameraComponent = InCameraActorComponent; }
	UCameraActorComponent* GetCameraComponent() { return CameraComponent; }

	// Weapon list
	UPROPERTY()
	TArray<AK_YGGun*> CurrentPlayerWeaponList;
	int16 MaxWeaponListSize = 3;
	// number of player currently having in weapon list
	int16 CurrentWeaponCount = 0;

	// Currently selected weapon index number
	int16 SelectWeaponIndex = 0;

	// Dummy weapon to express empty
	UPROPERTY()
	AK_YGGun* DummyGun;

	// Player Controller
	APlayerController* PlayerController;

	// when player character killed, drop all item
	void DropAllWeapon();



	// ----------- server function

	// To server send fire action and montage
	UFUNCTION(Server, Reliable)
	void ServerInputStartFire();

	UFUNCTION(Server, Reliable)
	void ServerInputStopFire();

	// To server send reload action and montage
	UFUNCTION(Server, Reliable)
	void ServerInputReload();

	// To server send Aim action and montage
	UFUNCTION(Server, Reliable)
	void ServerEnterZoom();

	UFUNCTION(Server, Reliable)
	void ServerExitZoom();

	UFUNCTION(Server, Reliable)
	void ServerConvertDischarge();

	UFUNCTION(Server, Reliable)
	void ServerGiveDamage(float Damage);

	// update currently holding weapon (depend to SelectWeaponIndex)
	UFUNCTION(Server, Reliable)
	void ServerUpdateCurrentWeapon();

	// pick weaopn to server
	UFUNCTION(Server, Reliable)
	void ServerPickWeapon(EWeapon InteractWeaponKind);

	// drop weapon to server
	UFUNCTION(Server, Reliable)
	void ServerDropWeapon();

	// press num '1' key: select main weapon 
	UFUNCTION(Server, Reliable)
	void ServerSelectNum1(FWeaponInfo SelectGunInfo);

	// press num '2' key: select sub weapon 
	UFUNCTION(Server, Reliable)
	void ServerSelectNum2(FWeaponInfo SelectGunInfo);

	// press num '3' key: select other weapon 
	UFUNCTION(Server, Reliable)
	void ServerSelectNum3(FWeaponInfo SelectGunInfo);

	UFUNCTION(Server, Reliable)
	void ServerDestroyWeapon(AK_YGGun* InWeaponToDestroy);



	// multicast 
	UFUNCTION(NetMulticast, Reliable)
	void MulticastInputStartFire();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastInputStopFire();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastInputReload();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEnterZoom();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastExitZoom();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastConvertDischarge();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastGiveDamage(float Damage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateCurrentWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPickWeapon(EWeapon InWeaponKind);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDropWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSelectNum1(FWeaponInfo InSelectGunInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSelectNum2(FWeaponInfo InSelectGunInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSelectNum3(FWeaponInfo InSelectGunInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroyWeapon(AK_YGGun* InWeaponToDestroy);


};
