// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "CameraActorComponent.generated.h"

// Player's camera view mode
UENUM(BlueprintType)
enum class EViewMode : uint8
{
	FIRST_PERSON,
	THIRD_PERSON,
	THIRD_PERSON_FOCUS_ON,
	AIM_ON
};

class ABaseWeapon;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UCameraActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Change player's camera view mode
	void ChangeViewMode(EViewMode NewViewMode);

	// When camera's transform is moving smooth with lerp
	void SmoothCameraMoving(bool bChangeToFocusOn);

	// View mode's getter and setter.
	void SetViewMode(EViewMode NewViewMode) { CurrentViewMode = NewViewMode; }
	EViewMode GetViewMode() { return CurrentViewMode; }

	// Camera's moving - up & down
	void CameraUpDownMoving(float MouseMovingDataPitch);

	// Camera's moving - left & right
	void CameraLeftRightMoving(float MouseMovingDataYaw);

	// perform line trace following camera's view
	void PerformLineTrace();

	// if this line hit with weapon or not
	bool bIsHovered = false;

	// enable and disable outline with weapons
	void EnableOutline(AActor* TargetActor, bool bEnable);
	void DisableAllOutlines();

	ABaseWeapon* GetHoveredWeaponClass() { return HoveredWeapon; }

public:

	// Player's camera
	UPROPERTY(VisibleAnywhere, Category = "K_YG|Camera")
	UCameraComponent* CameraComponent;

	// Camera's spring arm
	UPROPERTY(VisibleAnywhere, Category = "K_YG|Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY()
	ABaseWeapon *HoveredWeapon;

	TArray<AActor*> PreviouslyOutlinedActors;

	// zooming

	// Scene Capture Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComponent;

	// Render Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SniperScope", meta = (AllowPrivateAccess = "true"))
	UTextureRenderTarget2D* RenderTarget;


private:


	// Current player's view mode
	UPROPERTY(EditAnywhere, Category = "K_YG|Camera", Meta = (AllowPrivateAccess = true))
	EViewMode CurrentViewMode;

	// Current player's controller
	APlayerController* PlayerController;

	// Camera rotation view locked
	bool bIsCameraRotationViewLocked = false;


};

