// © 2024 Team 'K_YangGaeng'. All rights reserved.

#include "UI/GameResult/GameResultCharacter.h"
#include "UI/GameResult/GameResultComponent.h"

// Sets default values
AGameResultCharacter::AGameResultCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GameResultComponent를 생성하여 캐릭터에 부착
	GameResultComponent = CreateDefaultSubobject<UGameResultComponent>(TEXT("GameResultComponent"));
}

// Called when the game starts or when spawned
void AGameResultCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGameResultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGameResultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
