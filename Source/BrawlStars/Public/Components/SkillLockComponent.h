#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillLockComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNormalSkillFinished);
DECLARE_MULTICAST_DELEGATE(FOnUltimateSkillFinished);
DECLARE_MULTICAST_DELEGATE(FOnUltimateRechage);

USTRUCT(BlueprintType)
struct FSkillState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsSkillPressEnable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsSkillReleaseEnable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsNormalReady;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsNormalActivated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsNormalEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUltimateReady;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUltimateActivated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUltimateEnd;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAWLSTARS_API USkillLockComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AHeroBase;
public:	
	USkillLockComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnUltimateRechage UltimateRechage;
	FOnNormalSkillFinished NormalSkillFinished;
	FOnUltimateSkillFinished UltimateSkillFinished;

private:
	bool CheckPhysicsBottonNormal(bool bPress);
	bool CheckActivatableNormal();
	bool CheckReleaseableNormal();

	void RechageUltimateSkill();
	void ResetNormalLock();
	void ResetUltimateLock();
	void NormalBulletRechage();

	void CheckNormalSkillReady();
	void RechageNormalSkill();
	void CheckUltimateSkillReady();

	UFUNCTION(Server, Reliable)
	void ServerSetSkillReleasable(const FSkillState& SkillReleasable);

	UFUNCTION(Server, Reliable)
	void ServerSetSkillAmountCurrent(float SkillAmountCurrent);

public:
	FSkillState SkillState;

private:

	float NormalSkillMax;
	float NormalSkillCurrent;
	float NormalSKillRechageTime;
	FTimerHandle NormalSkillRechageTimer;

	float UltimateEnergyDefault;
	float UltimateEnergyCurrent;

	bool bNormalButtonPressed;
	bool bNormalButtonReleased = true;;

	bool bUltimateButtonPressed;
	bool bUltimateButtonReleased = true;
};
