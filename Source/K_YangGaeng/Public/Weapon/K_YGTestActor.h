// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/K_YGGun.h"
#include "K_YGTestActor.generated.h"

UCLASS()
class K_YANGGAENG_API AK_YGTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AK_YGTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Attack();

private:
	UPROPERTY(VisibleAnywhere)
	AK_YGGun* TestGun; 

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;
};
