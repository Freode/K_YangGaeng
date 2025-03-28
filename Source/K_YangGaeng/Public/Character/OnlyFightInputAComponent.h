// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "OnlyFightInputAComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnInputActionValue, const FInputActionValue&);
DECLARE_DELEGATE(FOnInputNoParams);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UOnlyFightInputAComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOnlyFightInputAComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Bind functions to input system
	UFUNCTION()
	void SetInputSystem(class UInputComponent* PlayerInputComponent);

private:

	// Bind to InputComponent methods

	// Moving
	void BindingMove(const FInputActionValue& Value);

	// Jumping
	void BindingStartJump();
	void BindingStopJump();

	// Sit
	void BindingSit(const FInputActionValue& Value);

	// Mouse Moving - View Move
	void BindingMouseMoving(const FInputActionValue& Value);

	// Dash Skill
	void BindingDashSkill();



	// ----------- yeonkyeong's gun function delegate
	void BindingStartFire();
	void BindingStopFire();
	void BindingReload();
	void BindingConvertDischarge();
	void BindingStartAim();
	void BindingStopAim();

	// select item idex with number key '1', '2', '3'
	void BindingNum1();
	void BindingNum2();
	void BindingNum3();

	// Throw weapon away with 'Q'
	void BindingDrop();

	// open shop with 'B'
	void BindingShop();

public:

	// Input system with delegate 
	FOnInputActionValue OnInputMove;
	FOnInputActionValue OnInputSit;
	FOnInputActionValue OnInputMouseViewChange;
	FOnInputNoParams OnInputStartJump;
	FOnInputNoParams OnInputStopJump;
	FOnInputNoParams OnInputDashSkill;

	// ----------- yeonkyeong's gun function delegate

	FOnInputNoParams OnInputStartFire;
	FOnInputNoParams OnInputStopFire;
	FOnInputNoParams OnInputReload;
	FOnInputNoParams OnInputConvertDischarge;
	FOnInputNoParams OnInputStartAim;
	FOnInputNoParams OnInputStopAim;

	// item select input ('1', '2')
	FOnInputNoParams OnInputNum1;
	FOnInputNoParams OnInputNum2;
	FOnInputNoParams OnInputNum3;

	// Throw weapon away with 'Q'
	FOnInputNoParams OnInputDrop;

	// open shop with 'B'
	FOnInputNoParams OnInputShop;

private:

	// Default mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputMappingContext* DefaultMappingContext;

	// Input interaction - Jump action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* JumpAction;

	// Input interaction - Move action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* MoveAction;

	// Input interaction - Look action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* LookAction;

	// Input interaction - Sit action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* SitAction;

	// Input interaction - Dash skill
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* DashSkillAction;


	// ----------- yeonkyeong's gun function delegate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* StartFireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* ConvertDischargeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* Num1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* Num2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* Num3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* DropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Input", meta = (AllowPrivateAccess = true))
	class UInputAction* ShopAction;


};

