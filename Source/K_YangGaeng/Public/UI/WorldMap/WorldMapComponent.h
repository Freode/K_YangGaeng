// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "WorldMapComponent.generated.h"

class UUserWidget;
class UWorldMapWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UWorldMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// 기본 생성자
	UWorldMapComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

};
