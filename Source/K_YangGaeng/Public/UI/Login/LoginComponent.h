// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "LoginComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class K_YANGGAENG_API ULoginComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 컴포넌트의 기본 값을 설정
	ULoginComponent();

protected:
	// 게임 시작 시 호출되는 함수
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LoginWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CreditWidgetClass;

private:
	// 위젯 인스턴스를 저장하기 위한 포인터
	UUserWidget* LoginWidget;
	UUserWidget* MainWidget;
	UUserWidget* CreditWidget;
};
