// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "GameFramework/Actor.h"
//#include "Weapon/BaseWeapon.h"
#include "K_YGBulletBase.generated.h"


class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class K_YANGGAENG_API AK_YGBulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AK_YGBulletBase();

protected:  
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function called when the bullet hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit); 
	
	// bullet collision component;
	UPROPERTY()
	USphereComponent* CollisionComponent;

	// bullet movement component
	UPROPERTY()
	UProjectileMovementComponent* BulletMovementComponent;

	// 총알 데미지
	float BulletDamage;		 
	ACharacter* OwnerCharacter;
	 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Set bullet damage
	void SetBulletDamage(float WeaponsBulletDamage) { BulletDamage = WeaponsBulletDamage; };

	// Set bullet's owner
	void SetCharacter(ACharacter* Character) { OwnerCharacter = Character; }
	ACharacter* GetCharacter() { return OwnerCharacter; }

	// Fire to direction
	void FireInDirection(float BulletVelocity, const FVector& FireDirection);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


	// static mesh for bullet body
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BulletMesh;

 

// Object Pooling codes
public:
	// object init
	void Initialize();		
	// activate object
	void Activate();		
	// deactivate
	void Deactivate();	

	// Destroy itself when they meet collision with enemy or map 
	bool bIsCollided = false;

	// Limit time that bullet can live maximum without collision
	float BulletMaxLifeTime = 5.0f;
	float BulletCurrentLifeTime = 0.0f;

	 
	bool bIsActive;

};
