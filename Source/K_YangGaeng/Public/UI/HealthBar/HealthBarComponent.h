// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthBarComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UHealthBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthBarComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* Character;


	void ServerDecreaseHealthWidget_Implementation(float Damage);

	void MulticastDecreaseHealthWidget_Implementation(float Damage);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 현재 체력을 설정: 체력을 새로운 값으로 갱신하고 체력 바 업데이트.
	void SetCurrentHealth(float NewHealth);

	// decrease health as taking damage
	void DecreaseHealth(float Damage);
	// change widget too if this client is local
	void DecreaseHealthWidget(float Damage);


	// server code
	UFUNCTION(Server, Reliable)
	void ServerDecreaseHealthWidget(float Damage);


	UFUNCTION(NetMulticast, Reliable)
	void MulticastDecreaseHealthWidget(float Damage);

	// getter 
	const float GetCurrentHealth() { return CurrentHealth; }

private:
	// 체력 바 업데이트: 새로운 현재 체력과 최대 체력을 사용
	void UpdateHealthBar(float NewCurrentHealth, float NewMaxHealth);

	// 체력을 50으로 감소
	void DecreaseHealthTo50();


	// execute character's death action
	void OnCharacterDeath();

	// 최대 체력 값
	UPROPERTY(EditAnywhere)
	float MaxHealth;

	// 현재 체력 값
	UPROPERTY(EditAnywhere)
	float CurrentHealth;

	// UI 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HealthBarWidgetClass;

	// 체력 바 위젯 인스턴스
	UPROPERTY()
	class UK_YGHPBarWidget* HealthBarWidget;

	// 타이머 핸들
	FTimerHandle TimerHandle;

};
