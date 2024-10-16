// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "GameFramework/Actor.h"
#include "AIMoveCenterLocation.generated.h"

class UBillboardComponent;

UCLASS()
class K_YANGGAENG_API AAIMoveCenterLocation : public AActor
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	AAIMoveCenterLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Searching random target which is AI character moving
	UFUNCTION(BlueprintCallable, Category = "K_YG|AI|Move")
	bool SearchingRandomTarget(FVector& OutResult);

	// Searching randomly with connect target lists
	UFUNCTION(BlueprintCallable, Category = "K_YG|AI|Move")
	bool SearchingRandomWithConnectList(FVector& OutResult);

	// Scene component for root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|AI|Move")
	USceneComponent* RootScene;

	// Billboard component for printing to viewport
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "K_YG|AI|Move")
	class UBillboardComponent* Sprite;

private:

	// Searching random position with this radius inner area
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|AI|Move", meta = (AllowPrivateAccess = true))
	float SearchingRadius;

	// Connect to another nearest AIMoveCenterLocation's objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|AI|Move", meta = (AllowPrivateAccess = true))
	TArray<AAIMoveCenterLocation*> ConnectTargetLists;

	// Previous target location which is ai character previous located
	UPROPERTY()
	AAIMoveCenterLocation* PreviousTargetLocation;
};
