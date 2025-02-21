// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameResultComponent.generated.h"

class UUserWidget;
class UWidget;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class K_YANGGAENG_API UGameResultComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameResultComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 위젯 및 하위 컴포넌트를 초기화하는 함수
	void InitializeWidget(UUserWidget* WidgetInstance);

private:
	// 게임 결과 위젯을 가리키는 멤버 변수
	UPROPERTY()
	UUserWidget* GameResultWidgetInstance;

	// 게임 결과 화면의 위젯
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UUserWidget* GameResultWidget;

	// 재귀적으로 위젯을 찾는 함수
	UWidget* FindWidgetRecursive(UWidget* ParentWidget, const FName& WidgetName);


	// =================================================================
	// =============================Server==============================
	// =================================================================
public:
	// 서버로 플레이어 수를 전송
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendNumberOfPlayer(int32 PlayerNumber);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastReceiveNumberOfPlayer(int32 PlayerNumber);

protected:

	// 서버에서 호출하기 위한 함수
	void ServerSendNumberOfPlayer_Implementation(int32 PlayerNumber);
	
	// 서버 유효성 검사 함수
	bool ServerSendNumberOfPlayer_Validate(int32 PlayerNumber);

	void MulticastReceiveNumberOfPlayer_Implementation(int32 PlayerNumber);




};
