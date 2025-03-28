// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

class AK_YGRifle;
class ABaseWeapon;

UCLASS()
class K_YANGGAENG_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnItem(FVector SpawnPosition, FRotator SpawnRotation);

	void SetItemClass(TSubclassOf<ABaseWeapon> NewItemClass){ItemClass = NewItemClass;}

	// The class of the item to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ABaseWeapon> ItemClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
