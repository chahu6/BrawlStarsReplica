#pragma once

#include "CoreMinimal.h"
#include "HeroBase.h"
#include "LaunchSkillOnly.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API ALaunchSkillOnly : public AHeroBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

protected:
	// 技能相关
	virtual void ActiveNormalSkill() override;
	virtual void ReleaseNormalSkill() override;

	virtual void ActiveUltimateSkill() override;
	virtual void ReleaseUltimateSkill() override;

private:
	FOnMontageEnded MontageEndedDelegate;

	UPROPERTY()
	TSubclassOf<ASkillBase> SkillClass;

	bool bIsNormalSkill = false;

	void ReleaseSkill(float InReleaseRotationOffset, bool bNormalSkill, UAnimMontage* InMontage, FName InMontageSection, TSubclassOf<ASkillBase> InSkillClass);

	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void UnbindDelegates();
};
