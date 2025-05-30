// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/GunInterface/K_YGGunInterfaceWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h" 
#include "Components/TextBlock.h"
#include "Weapon/K_YGGun.h"
 

void UK_YGGunInterfaceWidget::SetTotalBulletText(int16 TotalBullet)
{
	TotalBulletText->SetText(FText::AsNumber(TotalBullet));
}

void UK_YGGunInterfaceWidget::SetLoadedBulletText(int16 LoadedBullet)
{
	LoadedBulletText->SetText(FText::AsNumber(LoadedBullet));
}

void UK_YGGunInterfaceWidget::ChangeImage(int8 InSelectedWeaponIndex, UTexture2D* InWeaponTexture, float InAlphaValue)
{
    
	if (InterfaceImages[InSelectedWeaponIndex])
    {
        // update brush image
        FSlateBrush NewBrush;
        NewBrush.SetResourceObject(InWeaponTexture);
        FLinearColor NewColor = InterfaceImages[InSelectedWeaponIndex]->ColorAndOpacity;
        InterfaceImages[InSelectedWeaponIndex]->SetBrush(NewBrush);

        // update brush's alpha value (selected weapon is 1.0, other is 0.7)
        NewColor.A = FMath::Clamp(InAlphaValue, 0.0f, 1.0f); // 0.0 (투명) ~ 1.0 (불투명)
        InterfaceImages[InSelectedWeaponIndex]->SetColorAndOpacity(NewColor);

    }
    else  
    {
        K_YG_UELOG(Display, TEXT("Interfae Images list not valid"));
    } 
}
 

void UK_YGGunInterfaceWidget::NativeConstruct()
{
	Super::NativeConstruct();


	// make UImage list 
	InterfaceImages.Add(MainWeaponImage);
	InterfaceImages.Add(SubWeaponImage);
    InterfaceImages.Add(OtherWeaponImage);
}
 
