// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/HealthBar/HealthBarComponent.h"
#include "UI/HealthBar/K_YGHPBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UHealthBarComponent::UHealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 블루프린트 위젯 클래스 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarWidgetObj(TEXT("/Game/K_YangGaeng/UI/HPBar/WG_HPBar.WG_HPBar_C"));
	if (HealthBarWidgetObj.Succeeded())
	{
		HealthBarWidgetClass = HealthBarWidgetObj.Class;
	}

	// 기본 체력 값 설정
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

// Called when the game starts
void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		K_YG_UELOG(Error, TEXT("Character is nullptr"));
	}

	// set widget only if player is local
	if (HealthBarWidgetClass && Character->IsLocallyControlled())
	{
		HealthBarWidget = CreateWidget<UK_YGHPBarWidget>(GetWorld(), HealthBarWidgetClass);
		if (HealthBarWidget)
		{
			HealthBarWidget->AddToViewport();
			// 초기 체력 바 업데이트
			UpdateHealthBar(CurrentHealth, MaxHealth);
			// 초기 체력 텍스트 업데이트
			HealthBarWidget->SetHealthText(CurrentHealth);
		}
	}

	// 3초 후에 체력을 50으로 감소시키는 타이머 설정
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHealthBarComponent::DecreaseHealthTo50, 3.0f, false);
}

void UHealthBarComponent::ServerDecreaseHealthWidget_Implementation(float Damage)
{
	MulticastDecreaseHealthWidget(Damage);
}

void UHealthBarComponent::MulticastDecreaseHealthWidget_Implementation(float Damage)
{
	if (Character->IsLocallyControlled())
	{
		DecreaseHealthWidget(Damage);
	}
}

// Called every frame
void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 체력 바를 업데이트: 새로운 현재 체력과 최대 체력을 사용
void UHealthBarComponent::UpdateHealthBar(float NewCurrentHealth, float NewMaxHealth)
{
	if (HealthBarWidget)
	{
		K_YG_SIMPLE(Error);
		float HealthPercent = NewCurrentHealth / NewMaxHealth;
		HealthBarWidget->SetHealthPercent(HealthPercent);
		// 체력 텍스트 업데이트
		HealthBarWidget->SetHealthText(NewCurrentHealth);
	}
}

// 현재 체력을 설정: 체력을 새로운 값으로 갱신하고 체력 바를 업데이트
void UHealthBarComponent::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	UpdateHealthBar(CurrentHealth, MaxHealth);
}

// 체력을 50으로 감소시키는 함수
void UHealthBarComponent::DecreaseHealthTo50()
{
	SetCurrentHealth(50.0f);
}

void UHealthBarComponent::DecreaseHealth(float Damage)
{ 	
	// when character take damage larger than current health
	if (CurrentHealth <= Damage)
	{
		CurrentHealth = 0.0f;
		OnCharacterDeath();
		return;
	}

	CurrentHealth -= Damage;

}

void UHealthBarComponent::DecreaseHealthWidget(float Damage)
{	
	// when character take damage larger than current health
	if (CurrentHealth <= Damage)
	{
		SetCurrentHealth(0.0f);
		OnCharacterDeath();
		return;
	}
	SetCurrentHealth(CurrentHealth - Damage);
}

void UHealthBarComponent::OnCharacterDeath()
{
		K_YG_UELOG(Error, TEXT("Character die"));
}
