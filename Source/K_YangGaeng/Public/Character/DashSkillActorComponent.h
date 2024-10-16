// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "Components/ActorComponent.h"
#include "DashSkillActorComponent.generated.h"

class UAnimMontage;
class UNiagaraComponent;
class UAudioComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class K_YANGGAENG_API UDashSkillActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDashSkillActorComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Is dash skill playing?
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion|Skill|Dash")
	bool GetIsPlayingDashSkill(){ return bIsDashPlaying; }

	/**
	*	Play dash skill
	*	
	*	@return Succeed to playing dash skill
	*	@param InMoveDirection - Dash move to player input's direction 
	*/
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion|Skill|Dash")
	bool PlayDashSkill(const FVector2D InDashDirection);

	// Called when dash skill animation montage is finished to play
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion|Skill|Dash")
	void EndDashSkill();

	// Get progression time after operating dash skill
	float GetAfterUseDashTime() const { return AfterUseDashTime; }

	// Get operate dash skill is possible state
	bool GetCanOperateDashSkill() const { return bCanOperaterDashSkill; }

	// Attach dash effect on owner's character mesh
	void AttachDashEffectOnCharacterMesh();

	// =================================
	// ======== RPC - To Server ========

	// To Server - Dash skill operate direction
	UFUNCTION(Server, Reliable)
	void ServerOperateDashSkill(const FVector2D& InDashDirection);

	// To Server - End dash skill operate
	UFUNCTION(Server, Reliable)
	void ServerEndDashSkill();

	// =================================
	// ======== RPC - Multicast ========	

	// Multicast - Operate dash skill montage
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOperateDashSkill(const FVector2D& InDashDirection);

	// =================================
	// ======= Client --> Server =======

	// Check can operate dash skill?
	UFUNCTION(BlueprintCallable, Category = "K_YG|Locomotion|Skill|Dash")
	bool CheckOperateDashSkill(const FVector2D& InDashDirection);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// =================================
	// ======== RPC - To Server ========

	// To Server - Execute to can operate dash skill value
	void ServerOperateDashSkill_Implementation(const FVector2D& InDashDirection);

	// To Server - Execute to end dash skill
	void ServerEndDashSkill_Implementation();

	// =================================
	// ======== RPC - Multicast ========	

	// Multicast - Execute to operate dash skill montage
	void MulticastOperateDashSkill_Implementation(const FVector2D& InDashDirection);

private:

	/**
	*	Select dash skill anim montage which is based on player's input direction
	* 
	*	@param InMoveDirection - Dash move to player input's direction
	*	@param OutDashSkillMontage - Selected dash skill anim montage result
	*/
	void SelectDashSkillMontage(const FVector2D InDashDirection, class UAnimMontage* &OutDashSkillMontage);

	// Check player character which is attached this component isn't equal nullptr value
	void CheckPlayerCharacter();

	// Must call run on Server - Launch player character
	void LaunchCharacterFromDashSkill(const FVector2D& InDashDirection);

public:

	// Dash particle component
	UPROPERTY(VisibleAnywhere, Category = "K_YG|Locomotion|Skill|Dash")
	UNiagaraComponent* DashFX;

	// Dash sound component
	UPROPERTY(VisibleAnywhere, Category = "K_YG|Locomotion|Skill|Dash")
	class UAudioComponent* DashSound;

	// Forward dash animation montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion|Skill|Dash")
	class UAnimMontage* ForwardDashMontage;

	// Back dash animation montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion|Skill|Dash")
	class UAnimMontage* BackwardDashMontage;

	// Left dash animation montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion|Skill|Dash")
	class UAnimMontage* LeftDashMontage;

	// Right dash animation montage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion|Skill|Dash")
	class UAnimMontage* RightDashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion|Skill|Dash")
	FVector2D DashDirection = FVector2D(0.0f, 0.0f);

	// Dash skill's cool down time
	const float DashCooldownTime = 5.0f;

	// Total dash skill's power modification
	const float DashPowerModification = 0.125f;

	// Dash skill's power multiply when character is falling
	const float DashFallingPowerModification = 0.75f;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "K_YG|Locomotion|Skill|Dash", meta = (AllowPrivateAccess = true))
	float DashPower;

	// Character is playing dash interaction?
	bool bIsDashPlaying = false;

	// Owner's infromation which is character
	ACharacter* Character;
	
	// After last dash skill use time
	float AfterUseDashTime = 0.0f;

	// Is possible to operate dash skill?
	bool bCanOperaterDashSkill = true;


};
