// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Character/OnlyFightInputAComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"


// Sets default values for this component's properties
UOnlyFightInputAComponent::UOnlyFightInputAComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Default setting
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DEFAULTIMC(TEXT("/Game/K_YangGaeng/Characters/Inputs/MappingContexts/IMC_OnlyFightCharacter.IMC_OnlyFightCharacter"));
	if (DEFAULTIMC.Succeeded())
	{
		DefaultMappingContext = DEFAULTIMC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JUMPACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Jump.IA_Jump"));
	if (JUMPACTION.Succeeded())
	{
		JumpAction = JUMPACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MOVEACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Move.IA_Move"));
	if (MOVEACTION.Succeeded())
	{
		MoveAction = MOVEACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LOOKACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Look.IA_Look"));
	if (LOOKACTION.Succeeded())
	{
		LookAction = LOOKACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SITACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Sit.IA_Sit"));
	if (SITACTION.Succeeded())
	{
		SitAction = SITACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> DASHACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Dash.IA_Dash"));
	if (DASHACTION.Succeeded())
	{
		DashSkillAction = DASHACTION.Object;
	}


	// ----------------- yeonkyeong's InputAction 
	static ConstructorHelpers::FObjectFinder<UInputAction> FIREACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Fire.IA_Fire"));
	if (FIREACTION.Succeeded())
	{
		FireAction = FIREACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RELOADACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Reload.IA_Reload"));
	if (RELOADACTION.Succeeded())
	{
		ReloadAction = RELOADACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> CONVERTDISCHARGEACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_ConvertDischarge.IA_ConvertDischarge"));
	if (CONVERTDISCHARGEACTION.Succeeded())
	{
		ConvertDischargeAction = CONVERTDISCHARGEACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AIMACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Aim.IA_Aim"));
	if (AIMACTION.Succeeded())
	{
		AimAction = AIMACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> NUM1ACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Num1.IA_Num1"));
	if (NUM1ACTION.Succeeded())
	{
		Num1Action = NUM1ACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> NUM2ACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Num2.IA_Num2"));
	if (NUM2ACTION.Succeeded())
	{
		Num2Action = NUM2ACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> NUM3ACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Num3.IA_Num3"));
	if (NUM3ACTION.Succeeded())
	{
		Num3Action = NUM3ACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> DROPACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Drop.IA_Drop"));
	if (DROPACTION.Succeeded())
	{
		DropAction = DROPACTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SHOPACTION(TEXT("/Game/K_YangGaeng/Characters/Inputs/Actions/IA_Shop.IA_Shop"));
	if (SHOPACTION.Succeeded())
	{
		ShopAction = SHOPACTION.Object;
	}
}


// Called when the game starts
void UOnlyFightInputAComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// Add input mapping context
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn != nullptr)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Pawn->Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}


// Called every frame
void UOnlyFightInputAComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Bind functions to input system
void UOnlyFightInputAComponent::SetInputSystem(UInputComponent* PlayerInputComponent)
{
	// Get enhenced input component
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	K_YG_CHECK(EnhancedInputComponent != nullptr);

	// === Binding functions ===
	
	// Move
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UOnlyFightInputAComponent::BindingMove);

	// Jump
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingStartJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &UOnlyFightInputAComponent::BindingStopJump);

	// Sit
	EnhancedInputComponent->BindAction(SitAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingSit);

	// Mouse Moving
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UOnlyFightInputAComponent::BindingMouseMoving);

	// Dash Skill
	EnhancedInputComponent->BindAction(DashSkillAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingDashSkill);



	// ----------- yeonkyeong's gun function delegate
	
	// Start Fire
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingStartFire);

	//Stop Fire
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UOnlyFightInputAComponent::BindingStopFire);

	// Reload
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingReload);

	// Conver Discharge
	EnhancedInputComponent->BindAction(ConvertDischargeAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingConvertDischarge);

	// Start Aim
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingStartAim);

	// Stop Aim
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &UOnlyFightInputAComponent::BindingStopAim);
 
	// num '1'
	EnhancedInputComponent->BindAction(Num1Action, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingNum1);

	// num '2'
	EnhancedInputComponent->BindAction(Num2Action, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingNum2);

	// num '2'
	EnhancedInputComponent->BindAction(Num3Action, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingNum3);

	// Drop Weapon 
	EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingDrop);

	// Open/Close weapon shop
	EnhancedInputComponent->BindAction(ShopAction, ETriggerEvent::Started, this, &UOnlyFightInputAComponent::BindingShop);

}

// move specific direction methods which are binding to InputComponent
void UOnlyFightInputAComponent::BindingMove(const FInputActionValue& Value)
{
	if (OnInputMove.IsBound())
	{
		OnInputMove.Execute(Value);
	}
}

// Jumping start
void UOnlyFightInputAComponent::BindingStartJump()
{
	if (OnInputStartJump.IsBound())
	{
		OnInputStartJump.Execute();
	}
}

// Jumping stop
void UOnlyFightInputAComponent::BindingStopJump()
{
	if (OnInputStopJump.IsBound())
	{
		OnInputStopJump.Execute();
	}
}

// Bind to InputComponent methods - Sit
void UOnlyFightInputAComponent::BindingSit(const FInputActionValue& Value)
{
	if (OnInputSit.IsBound())
	{
		OnInputSit.Execute(Value);
	}
}

// Mouse Moving - View Move
void UOnlyFightInputAComponent::BindingMouseMoving(const FInputActionValue& Value)
{
	if (OnInputMouseViewChange.IsBound())
	{
		OnInputMouseViewChange.Execute(Value);
	}
}

// Dash Skill
void UOnlyFightInputAComponent::BindingDashSkill()
{
	if (OnInputDashSkill.IsBound())
	{
		OnInputDashSkill.Execute();
	}
}




// ----------- yeonkyeong's gun function delegate
void UOnlyFightInputAComponent::BindingStartFire()
{
	if (OnInputStartFire.IsBound())
	{
		OnInputStartFire.Execute();
	}
}

void UOnlyFightInputAComponent::BindingStopFire()
{
	if (OnInputStopFire.IsBound())
	{
		OnInputStopFire.Execute();
	}
}

void UOnlyFightInputAComponent::BindingReload()
{
	if (OnInputReload.IsBound())
	{
		OnInputReload.Execute();
	}
}

void UOnlyFightInputAComponent::BindingConvertDischarge()
{
	if (OnInputConvertDischarge.IsBound())
	{
		OnInputConvertDischarge.Execute();
	}
}

void UOnlyFightInputAComponent::BindingStartAim()
{
	if (OnInputStartAim.IsBound())
	{
		OnInputStartAim.Execute();
	}
}

void UOnlyFightInputAComponent::BindingStopAim()
{
	if (OnInputStopAim.IsBound())
	{
		OnInputStopAim.Execute();
	}
}

void UOnlyFightInputAComponent::BindingNum1()
{
	if (OnInputNum1.IsBound())
	{
		OnInputNum1.Execute();
	}

}

void UOnlyFightInputAComponent::BindingNum2()
{
	if (OnInputNum2.IsBound())
	{
		OnInputNum2.Execute();
	}
}

void UOnlyFightInputAComponent::BindingNum3()
{
	if (OnInputNum3.IsBound())
	{
		OnInputNum3.Execute();
	}
}


void UOnlyFightInputAComponent::BindingDrop()
{
	if (OnInputDrop.IsBound())
	{
		OnInputDrop.Execute();
	}
}

void UOnlyFightInputAComponent::BindingShop()
{
	if (OnInputShop.IsBound())
	{
		OnInputShop.Execute();
	}
}
