#pragma once

#include "CoreMinimal.h"
#include "HeroBase.h"
#include "FlatSkillOnly.generated.h"

class ASkillBase;
/**
 * 
 */
UCLASS()
class BRAWLSTARS_API AFlatSkillOnly : public AHeroBase
{
	GENERATED_BODY()
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

protected:
	// �������
	virtual void ActiveNormalSkill() override;
	virtual void ReleaseNormalSkill() override;

	virtual void ActiveUltimateSkill() override;
	virtual void ReleaseUltimateSkill() override;

	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void ReleaseSkill(float InReleaseRotationOffset, bool bNormalSkill, UAnimMontage* InMontage, FName InMontageSection, TSubclassOf<ASkillBase> InSkillClass);

private:
	UFUNCTION(Server, Reliable)
	void ServerReleaseSkill(const FAimInfo& AimInfo, float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> InSkillClass);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastReleaseSkill(const FAimInfo& AimInfo, float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> InSkillClass);

	void UnbindDelegates();

private:
	FOnMontageEnded MontageEndedDelegate;

	UPROPERTY()
	TSubclassOf<ASkillBase> SkillClass;

	bool bIsNormalSkill = false;
};
