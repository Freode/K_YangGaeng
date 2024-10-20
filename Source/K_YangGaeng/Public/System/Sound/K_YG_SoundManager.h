// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../../K_YangGaeng.h"
#include "GameFramework/Actor.h"
#include "K_YG_SoundManager.generated.h"

UCLASS()
class K_YANGGAENG_API AK_YG_SoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's Tproperties
	AK_YG_SoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetMusicVolume(float Volume);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetSoundEffectsVolume(float Volume);

};
