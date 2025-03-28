// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "UI/K_YG_TeamMatchingActor.h"
#include "UI/WG_TeamMatching.h"

// Sets default values
AK_YG_TeamMatchingActor::AK_YG_TeamMatchingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//#. 이 Actor를 가진 객체에 Collision 제작
	CollisionComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = CollisionComponent;

	//#. 이 Actor와 오버랩(겹침)된 객체가 일으킬 함수 설정 
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AK_YG_TeamMatchingActor::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AK_YG_TeamMatchingActor::OnOverlapEnd);

	//#. TeamMatch widget class 가져오기
	static ConstructorHelpers::FClassFinder<UUserWidget> TeamMatchingClassFinder(TEXT("/Game/K_YangGaeng/UI/WG_TeamMatching.WG_TeamMatching_C"));
	if (TeamMatchingClassFinder.Succeeded()) {
		TeamMatchWidgetClass = TeamMatchingClassFinder.Class;
	}

	//#. 형태 추가 (현재는 cube)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		CollisionComponent->SetStaticMesh(CubeMeshAsset.Object);
	}

}

// Called when the game starts or when spawned
void AK_YG_TeamMatchingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AK_YG_TeamMatchingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AK_YG_TeamMatchingActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	K_YG_SIMPLE(Warning);
	//#. playercontroller 가져와서
	//#. character뽑아서 or Actor뽑아서 아래 식에 넣기
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (OtherActor == PlayerCharacter && TeamMatchWidgetClass)
	{
		K_YG_SIMPLE(Warning);

		// Create and show the widget
		if (!TeamMatchWidget)
		{
			K_YG_SIMPLE(Warning);

			TeamMatchWidget = CreateWidget<UWG_TeamMatching>(GetWorld(), TeamMatchWidgetClass);
			if (TeamMatchWidget)
			{
				TeamMatchWidget->AddToViewport();
			}
		}
	}
}

void AK_YG_TeamMatchingActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	K_YG_SIMPLE(Warning);
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (OtherActor == PlayerCharacter && TeamMatchWidgetClass)
	{
		K_YG_SIMPLE(Warning);

		// Create and show the widget
		if (TeamMatchWidget)
		{
			K_YG_SIMPLE(Warning);
			TeamMatchWidget->RemoveFromParent();
			TeamMatchWidget = nullptr;
		}

	}
}
