// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Character/CameraActorComponent.h"
#include "Weapon/BaseWeapon.h"

// Sets default values for this component's properties
UCameraActorComponent::UCameraActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...

	// Assign components
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

}


// Called when the game starts
void UCameraActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	PlayerController = Cast<APlayerController>(Character->GetController());
}


// Called every frame
void UCameraActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	PerformLineTrace();
}

void UCameraActorComponent::ChangeViewMode(EViewMode NewViewMode)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	switch (NewViewMode)
	{

	case EViewMode::FIRST_PERSON:
		K_YG_SIMPLE(Warning);
		break;

	// Set third person character's view
	case EViewMode::THIRD_PERSON:

		// Hierarchy
		SpringArmComponent->SetupAttachment(Character->GetCapsuleComponent());
		CameraComponent->SetupAttachment(SpringArmComponent);

		// Set camera's setting
		// Camera doesn't reflect character mesh's rotation
		CameraComponent->bUsePawnControlRotation = false;

		// Set spring arm's setting
		// Spring Arm  doesn't reflect character mesh's rotation. Because spring arm component is attached to character mesh. So, it was reflected already.
		SpringArmComponent->bUsePawnControlRotation = true;
		SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 125.0f));
		SpringArmComponent->TargetArmLength = 400.0f;
		break;

	// Set low focus aim on when third person charater's view. But it isn't high focus mode.
	case EViewMode::THIRD_PERSON_FOCUS_ON:
		K_YG_SIMPLE(Warning);
		break;

	// Set high focus on mode. This is 
	case EViewMode::AIM_ON:
		K_YG_SIMPLE(Warning);
		break;

	}
	// Change to new
	SetViewMode(NewViewMode);
}

// When camera's transform is moving smooth with lerp
void UCameraActorComponent::SmoothCameraMoving(bool bChangeToFocusOn)
{

}

// Current player's view mode
void UCameraActorComponent::CameraUpDownMoving(float MouseMovingDataPitch)
{
	FRotator NewRotation = PlayerController->GetControlRotation();
	
	// Prevent over & under value like -270 Degree = 90 Degree
	if (NewRotation.Pitch > 270.0f) { NewRotation.Pitch -= 360.0f; }
	else if (NewRotation.Pitch < -270.0f) { NewRotation.Pitch += 360.0f; }

	// Player's view rotation is changed in limitation
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + MouseMovingDataPitch * 45.0f * GetWorld()->GetDeltaSeconds(), -60.0f, 60.0f);
	PlayerController->SetControlRotation(NewRotation);
}

// Camera's moving - left & right
void UCameraActorComponent::CameraLeftRightMoving(float MouseMovingDataYaw)
{
	FRotator NewRotation = PlayerController->GetControlRotation();
	// Player's view yaw rotation is changed
	NewRotation.Yaw = NewRotation.Yaw + MouseMovingDataYaw;
	
	// Prevent over & under value
	if (NewRotation.Yaw >= 180.0f)
	{
		NewRotation.Yaw -= 360.0f;
	}
	else if (NewRotation.Yaw < -180.0f)
	{
		NewRotation.Yaw += 360.0f;
	}

	PlayerController->SetControlRotation(NewRotation);
}

void UCameraActorComponent::PerformLineTrace()
{
	FVector Start = CameraComponent->GetComponentLocation();  
	FVector ForwardVector = CameraComponent->GetForwardVector();
	FVector End = Start + (ForwardVector * 1000.0f);  

	FHitResult HitResult;

	AActor* HitActor;

	// line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility  
	);
	 
	if (bHit)
	{ 
		HitActor = HitResult.GetActor();
		UClass* ActorClass = HitActor->GetClass();

		// 특정 클래스의 자식인지 확인
		if (ActorClass->IsChildOf(ABaseWeapon::StaticClass()))
		{
			// execute bello codes only onec when bIsHoverd be true
			if(bIsHovered == false)
			{
				bIsHovered = true;
				HoveredWeapon = Cast<ABaseWeapon>(HitActor);
				EnableOutline(HitResult.GetActor(), true);
				K_YG_UELOG(Error, TEXT("Hit with weapon"));

			}
		} 
		else
		{
			bIsHovered = false;
			DisableAllOutlines();
		}
		// draw sphere 
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Red, false, 1.0f);
		//DisableAllOutlines();
	} 

	// draw debug line
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 10.0f);
}

void UCameraActorComponent::EnableOutline(AActor* TargetActor, bool bEnable)
{
	K_YG_SIMPLE(Warning);
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(TargetActor->GetRootComponent());

	if (PrimitiveComponent)
	{
		PrimitiveComponent->SetRenderCustomDepth(bEnable);
		if (bEnable)
		{  
			// add to outlined actors array (to disable outline after)
			PreviouslyOutlinedActors.Add(TargetActor);
		}
	}
}

void UCameraActorComponent::DisableAllOutlines()
{
	// find all actor that outlined previously and deactivate all of it
	for (AActor* Actor : PreviouslyOutlinedActors)
	{
		EnableOutline(Actor, false);
	}

	PreviouslyOutlinedActors.Empty();
}

