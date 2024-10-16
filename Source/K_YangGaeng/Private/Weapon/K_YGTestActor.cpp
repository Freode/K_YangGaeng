// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGTestActor.h"

// Sets default values
AK_YGTestActor::AK_YGTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = Body;

}

// Called when the game starts or when spawned
void AK_YGTestActor::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void AK_YGTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AK_YGTestActor::Attack()
{
	TestGun->Fire();
}

