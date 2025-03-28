// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../K_YangGaeng.h"
#include "GameFramework/Actor.h"
#include "K_YG_TeamMatchingActor.generated.h"

class UUserWidget;
class UPrimitiveComponent;
class AActor;
class UStaticMeshComponent;

UCLASS()
class K_YANGGAENG_API AK_YG_TeamMatchingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AK_YG_TeamMatchingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> TeamMatchWidgetClass;

	// The widget instance
	UPROPERTY()
	UUserWidget* TeamMatchWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	UStaticMeshComponent* CollisionComponent;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function to handle overlap end
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
