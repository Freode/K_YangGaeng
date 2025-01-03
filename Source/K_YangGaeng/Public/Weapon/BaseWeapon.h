// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "../../K_YangGaeng.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"


class AK_YGTestOnlyFightCharacter;

UENUM(BlueprintType)
enum class EWeapon : uint8
{
	NONE,			// 무기 없음
	HANDGUN,		// 권총
	SMG,			// 기관단총
	RIFLE,			// 소총
	SNIPERRIFLE,	// 저격소총
	SHOTGUN,		// 산탄총
	KNIFE,			// 근접무기
	GRENADE			// 수류탄
};

// sound or particle effect informations to play
USTRUCT(BlueprintType)
struct FWeaponEffectInfo
{
	GENERATED_BODY();

	// Gun fire sound to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* FireSound;

	// knife attack(slash) sound to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* SlashSound;

	// Gun reload sound to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* ReloadSound;

	// Particle system to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* FireParticleSystem;
};


UCLASS()
class K_YANGGAENG_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckPlayerCharacter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// set owner character class(to access camera component ect)
	void SetOwnerCharacterClass(AK_YGTestOnlyFightCharacter* OwnerCharacterClass) { CharacterClass = OwnerCharacterClass;}




public:

	void EWeaponToString(EWeapon WeaponKind);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* WeaponBody;

protected:
	//enum class EWeapon
	//{
	//	HANDGUN,		// 권총
	//	SMG,			// 기관단총
	//	RIFLE,			// 소총
	//	SNIPERRIFLE,	// 저격소총
	//	SHOTGUN,		// 산탄총
	//	KNIFE,			// 근접무기
	//	GRENADE			// 수류탄
	//};


protected:
	// Player character object which is attached this parkour component
	UPROPERTY(BlueprintReadOnly, Category = "K_YG|Locomotion|Parkour", meta = (AllowPrivateAccess = true))
	class ACharacter* Character;


	UPROPERTY()
	UAnimInstance* CharacterAnimInstance;

	FOnMontageEnded MontageEndedDelegate;

	// after special montage end, then return to idle montage
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY()
	AK_YGTestOnlyFightCharacter* CharacterClass;




	// play different montage with each weapons
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* UpperBodyIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* UpperBodyFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* UpperBodyReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* UpperBodyAim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* UpperBodyOriginalIdle;

	// set weapon's collision block to catch line trace when weapon spawned in map
	void SetCollisionBlock();

	// set weapon's collision overlap to move player properly when they're having weapon
	void SetCollisionOverlap();

};
