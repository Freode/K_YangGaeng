 // © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "System/Sound/K_YG_SoundManager.h"

// Sets default values
AK_YG_SoundManager::AK_YG_SoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AK_YG_SoundManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AK_YG_SoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AK_YG_SoundManager::SetMusicVolume(float Volume)
{
	//if (MusicSoundClass)
	//{
	//	UGameplayStatics::SetSoundClassVolume(MusicSoundClass, Volume);
	//}
}

void AK_YG_SoundManager::SetSoundEffectsVolume(float Volume)
{
	//if (SoundEffectsSoundClass)
	//{
	//	UGameplayStatics::SetSoundClassVolume(SoundEffectsSoundClass, Volume);
	//}
}

