// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "GunInterfaceComponent.generated.h"

class AK_YGGun;
class UImage;
class UTexture2D;

enum class EWeapon : uint8;
struct FWeaponInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UGunInterfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunInterfaceComponent();


	// init texture
	bool InitializeComponents();

	bool CheckTextureIsValid();

	// set image 
	void SetWeaponImage(int idx, EWeapon WeaponKind, float InAlphaValue);

 
	// update bullets text
	void UpdateBulletText(int NewLoadedBulletText, int NewTotalBulletText);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// init interface
	void InitInterface();

	// set weapon in (idx)th place
	void SetWeapon(int idx, FWeaponInfo* WeaponInfo);

	UPROPERTY()
	ACharacter* Character;

	// bind images widget  
	UPROPERTY(meta = (BindWidget))
	TArray<UImage*> InterfaceImages;

	// textures 
	UPROPERTY()
	UTexture2D* EmptyTexture;

	UPROPERTY()
	UTexture2D* RifleTexture;

	UPROPERTY()
	UTexture2D* HandGunTexture;

	UPROPERTY()
	UTexture2D* SniperRifleTexture;

	UPROPERTY()
	UTexture2D* SMGTexture;

	UPROPERTY()
	UTexture2D* ShotGunTexture;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
 


	UPROPERTY(EditAnywhere)
	int TotalBullet;

	UPROPERTY(EditAnywhere)
	int LoadedBullet;

	// UI widget class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> GunInterfaceWidgetClass;

	// gun interface widget instance
	UPROPERTY()
	class UK_YGGunInterfaceWidget* GunInterfaceWidget;


};
