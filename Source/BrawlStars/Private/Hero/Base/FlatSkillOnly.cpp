#include "Hero/Base/FlatSkillOnly.h"

#include "Hero/Base/HeroType/AimingFlat.h"
#include "Kismet/KismetSystemLibrary.h"

void AFlatSkillOnly::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AFlatSkillOnly::BeginPlay()
{
	Super::BeginPlay();

	EquipFlatAimingManager();
}

void AFlatSkillOnly::ActiveNormalSkill()
{
	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = true;
	AimingManager.FlatAimingManager->CenterMousePosition();
}

void AFlatSkillOnly::ReleaseNormalSkill()
{
	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = false;
	ReleaseSkill(HeroSkills.NormalReleaseOffset, true, HeroSkills.NormalMontage, HeroSkills.NormalMontageSection, HeroSkills.NormalSkillType);
}

void AFlatSkillOnly::ActiveUltimateSkill()
{
	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = true;
	AimingManager.FlatAimingManager->CenterMousePosition();
}

void AFlatSkillOnly::ReleaseUltimateSkill()
{

}

void AFlatSkillOnly::ReleaseSkill(float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, const TSubclassOf<ASkillBase>& SkillType)
{
	AimingManager.FlatAimingManager->SkillMontageStop();
	AimingManager.FlatAimingManager->SetHeroSkillReleaseRotation(ReleaseRotationOffset);
	AimingManager.FlatAimingManager->LockMovementOrientRotation();
	AimingManager.FlatAimingManager->ClientPlayMontage(Montage, 1.0f, MontageSection);
	//GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AFlatSkillOnly::ReleaseFlatSkill);

	AimingManager.FlatAimingManager->ReleaseFlatSkill(SkillType);
}
