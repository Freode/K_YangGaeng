// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "AI/AIMoveCenterLocation.h"
#include "Components/BillboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAIMoveCenterLocation::AAIMoveCenterLocation()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create components
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    Sprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));

    // Set hiearchy
    RootComponent = RootScene;
    Sprite->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAIMoveCenterLocation::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AAIMoveCenterLocation::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Searching random target which is AI character moving
bool AAIMoveCenterLocation::SearchingRandomTarget(FVector& OutResult)
{
    FVector CenterLocation = GetActorLocation();

    // Get navigation system
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    K_YG_SIMPLE_CHECK(NavSys != nullptr, false);

    // Find random location within radius
    FNavLocation OutNavLocation;
    while (NavSys->GetRandomPointInNavigableRadius(CenterLocation, SearchingRadius, OutNavLocation) == false)
    {

    }

    OutResult = OutNavLocation.Location;

    return true;
}

// Searching randomly with connect target lists
bool AAIMoveCenterLocation::SearchingRandomWithConnectList(FVector& OutResult)
{
    int32 ListNum = ConnectTargetLists.Num();
    K_YG_SIMPLE_CHECK(ListNum != 0, false);

    int32 RandomIdx = 0;

    // Iterate until previous target & current target aren't equal
    do
    {
        // Random searching within connect target lists
        RandomIdx = FMath::RandRange(0, ListNum - 1);
    } while (PreviousTargetLocation == ConnectTargetLists[RandomIdx]);

    return ConnectTargetLists[RandomIdx]->SearchingRandomTarget(OutResult);
}
