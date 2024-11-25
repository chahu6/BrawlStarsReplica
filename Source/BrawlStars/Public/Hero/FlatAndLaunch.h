#pragma once

#include "CoreMinimal.h"
#include "HeroBase.h"
#include "FlatAndLaunch.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API AFlatAndLaunch : public AHeroBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// 技能相关
	virtual void ActiveNormalSkill() override;
	virtual void ReleaseNormalSkill() override;

	virtual void ActiveUltimateSkill() override;
	virtual void ReleaseUltimateSkill() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UltimateLaunchSkill();

private:
	void NormalFlatSkill();

	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
