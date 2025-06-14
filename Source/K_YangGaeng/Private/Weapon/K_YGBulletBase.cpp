// © 2024 Team 'K_YangGaeng'. All rights reserved.


#include "Weapon/K_YGBulletBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AK_YGBulletBase::AK_YGBulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = false;


	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));

	RootComponent = BulletMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Game/K_YangGaeng/Weapons/WeaponAssets/Bullet/SM_Bullet.SM_Bullet"));

	if (SM_BODY.Succeeded()) 
	{
		BulletMesh->SetStaticMesh(SM_BODY.Object);
	}

	BulletMesh->SetWorldScale3D(FVector(10.0f, 10.0f, 10.0f));
	//BulletMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//BulletMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	// set replicated option
	bReplicates = true;
	SetReplicateMovement(true);

	// Collision setup
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BulletMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BulletMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BulletMesh->SetNotifyRigidBodyCollision(true);  

	// Bind to the OnComponentHit event directly
	BulletMesh->OnComponentHit.AddDynamic(this, &AK_YGBulletBase::OnHit);

	//BulletMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

 

}

// Called when the game starts or when spawned
void AK_YGBulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AK_YGBulletBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{ 
	K_YG_SIMPLE(Display);
	// Only proceed if we are on the server
	// and replicate to other players
	if (HasAuthority()) 
	{ 
		// Other Actor and component  that hit with bullet is exist and not mine
		if (OtherActor && OtherActor != this && OtherComp)
		{

			// Apply damage to the actor if it's a character
			ACharacter* HitCharacter = Cast<ACharacter>(OtherActor); 

			// if hitted character is who fired a bullet then do nothing and return
			if (HitCharacter && HitCharacter == GetCharacter() || !HitCharacter)
			{ 
				return;
			}

			// if not, then apply damage to hitted player
			K_YG_UELOG(Error, TEXT("Hit other player"));
			// aplly damage to character
			UGameplayStatics::ApplyDamage(HitCharacter, BulletDamage, GetInstigatorController(), this, nullptr);

			// deactivate after collision occure
			Deactivate();
		}
	}
}
 
// Called every frame
void AK_YGBulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// when the bullet is activated
	if (bIsActive)  
	{

		// get impulse toward direction
		// ... 

		BulletCurrentLifeTime += DeltaTime;
		// if bullet consume all life time they have, deactivate itself
		if (BulletCurrentLifeTime >= BulletMaxLifeTime)
		{ 
			BulletCurrentLifeTime = 0.0f;
			Deactivate();
		}
	}


}

// Add Force to bullet to fire
void AK_YGBulletBase::FireInDirection(float BulletVelocity, const FVector& FireDirection)
{
	if (BulletMesh)
	{
		FVector Force = FireDirection * BulletVelocity;
		BulletMesh->AddImpulse(Force);
	}
}

void AK_YGBulletBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	  
}


void AK_YGBulletBase::Initialize()
{
	// init logic
}


// set bullet visible in game
void AK_YGBulletBase::Activate()
{ 
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	BulletMesh->SetSimulatePhysics(true);
	BulletMesh->SetEnableGravity(true);
}

// set bullet invisible in game
void AK_YGBulletBase::Deactivate()
{
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	BulletMesh->SetSimulatePhysics(false);
	BulletMesh->SetEnableGravity(false);

}



