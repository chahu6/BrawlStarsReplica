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
	// 技能相关
	virtual void ActiveNormalSkill() override;
	virtual void ReleaseNormalSkill() override;

	virtual void ActiveUltimateSkill() override;
	virtual void ReleaseUltimateSkill() override;

	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void ReleaseSkill(float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> SkillType);

private:
	UFUNCTION(Server, Reliable)
	void ServerReleaseSkill(const FAimInfo& AimInfo, float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> SkillType);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastReleaseSkill(const FAimInfo& AimInfo, float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> SkillType);

	void UnbindDelegates();

private:
	FOnMontageEnded MontageEndedDelegate;

	TSubclassOf<ASkillBase> SkillClass;

	bool bIsNormalSkill = false;
};
