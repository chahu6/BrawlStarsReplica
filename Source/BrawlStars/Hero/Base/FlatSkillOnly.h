#pragma once

#include "CoreMinimal.h"
#include "HeroBase.h"
#include "FlatSkillOnly.generated.h"

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

private:
	void ReleaseSkill(float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, const TSubclassOf<ASkillBase>& SkillType);

	void ReleaseFlatSkill();
};
