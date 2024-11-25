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
	bool bIsSkillPressEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsSkillReleaseEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsNormalReady = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsNormalActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsNormalEnd = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUltimateReady = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUltimateActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUltimateEnd = true;
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

	bool CheckActivatableUltimate();
	bool CheckReleaseableUltimate();

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

protected:
	UPROPERTY()
	float NormalSkillMax;

	UPROPERTY(BlueprintReadOnly)
	float NormalSkillCurrent;

	UPROPERTY()
	float NormalSKillRechageTime;

	FTimerHandle NormalSkillRechageTimer;

	UPROPERTY()
	float UltimateEnergyDefault;

	UPROPERTY()
	float UltimateEnergyCurrent;

	UPROPERTY()
	bool bNormalButtonPressed;

	UPROPERTY()
	bool bNormalButtonReleased = true;

	UPROPERTY()
	bool bUltimateButtonPressed;

	UPROPERTY()
	bool bUltimateButtonReleased = true;
};
