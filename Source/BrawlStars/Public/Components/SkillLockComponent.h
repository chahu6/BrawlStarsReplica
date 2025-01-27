#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillLockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNormalSkillFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUltimateSkillFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUltimateRechage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValuesChangedSignature, float, CurrentValue, float, MaxValue);

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnUltimateRechage UltimateRechage;
	FOnNormalSkillFinished NormalSkillFinished;

	UPROPERTY(BlueprintCallable)
	FOnUltimateSkillFinished UltimateSkillFinished;

	UPROPERTY(BlueprintAssignable)
	FOnValuesChangedSignature NormalSkillValueChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangedSignature FOnUltimateEnergyChangedDelegate;

	void ResetUltimateEnergyCurrent();

private:
	bool CheckPhysicsBottonNormal(bool bPress);
	bool CheckActivatableNormal();
	bool CheckReleaseableNormal();

	bool CheckActivatableUltimate();
	bool CheckReleaseableUltimate();

	UFUNCTION()
	void RechageUltimateSkill();

	UFUNCTION()
	void ResetNormalLock();

	UFUNCTION()
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
	UPROPERTY(BlueprintReadOnly)
	FSkillState SkillState;

protected:
	UPROPERTY(BlueprintReadOnly)
	float NormalSkillMaxAmount;

	UPROPERTY(BlueprintReadOnly)
	float NormalSkillCurrentValue;

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
