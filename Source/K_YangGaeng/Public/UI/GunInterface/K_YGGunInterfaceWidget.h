// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Blueprint/UserWidget.h" 
#include "K_YGGunInterfaceWidget.generated.h"

/**
 * 
 */

class UImage;
class UTexture2D;

enum class EWeapon : uint8;
struct FWeaponInfo;

UCLASS()
class K_YANGGAENG_API UK_YGGunInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// set total bullets text
	void SetTotalBulletText(int16 TotalBullet);

	// set loaded bullets text
	void SetLoadedBulletText(int16 LoadedBullet);

	// set weaopn image texture
	void ChangeImage(int8 idx, UTexture2D* InTexture, float InAlphaValue); 

protected:

	
	virtual void NativeConstruct() override;


	// display total bullet amount
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalBulletText;

	// dislay loaded bullet amount 
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LoadedBulletText;

	// Main weapon image
	UPROPERTY(meta = (BindWidget))
	class UImage* MainWeaponImage;

	// sub weapon image
	UPROPERTY(meta = (BindWidget))
	class UImage* SubWeaponImage;

	// other weapon image
	UPROPERTY(meta = (BindWidget))
	class UImage* OtherWeaponImage;

	// bind images widget  
	UPROPERTY(meta = (BindWidget))
	TArray<UImage*> InterfaceImages;


};
