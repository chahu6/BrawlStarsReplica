#include "Hero/Base/FlatSkillOnly.h"
#include "Hero/Base/HeroType/AimingFlat.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkillLockComponent.h"

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
	ServerReleaseSkill(HeroSkills.NormalReleaseOffset, true, HeroSkills.NormalMontage, HeroSkills.NormalMontageSection, HeroSkills.NormalSkillType);
}

void AFlatSkillOnly::ActiveUltimateSkill()
{
	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = true;
	AimingManager.FlatAimingManager->CenterMousePosition();
}

void AFlatSkillOnly::ReleaseUltimateSkill()
{

}

void AFlatSkillOnly::ServerReleaseSkill_Implementation(float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> SkillType)
{
	MulticastReleaseSkill(ReleaseRotationOffset, bNormalSkill, Montage, MontageSection, SkillType);
}

void AFlatSkillOnly::MulticastReleaseSkill_Implementation(float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> SkillType)
{
	if (IsLocallyControlled()) return;

	ReleaseSkill(ReleaseRotationOffset, bNormalSkill, Montage, MontageSection, SkillType);
}

void AFlatSkillOnly::ReleaseSkill(float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> SkillType)
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr) return;

	AimingManager.FlatAimingManager->SkillMontageStop();
	AimingManager.FlatAimingManager->SetHeroSkillReleaseRotation(ReleaseRotationOffset);
	AimingManager.FlatAimingManager->LockMovementOrientRotation();
	AimingManager.FlatAimingManager->ClientPlayMontage(Montage, 1.0f, MontageSection);

	SkillClass = SkillType;
	bIsNormalSkill = bNormalSkill;

	MontageEndedDelegate.BindUObject(this, &AFlatSkillOnly::OnMontageEnded);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AFlatSkillOnly::OnNotifyBeginReceived);
}

void AFlatSkillOnly::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (!HasAuthority()) return;

	AimingManager.FlatAimingManager->ReleaseFlatSkill(SkillClass);
}

void AFlatSkillOnly::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		AimingManager.FlatAimingManager->UnlockMovementOrientRotation();
		if (bIsNormalSkill && SkillLockComponent)
		{
			SkillLockComponent->NormalSkillFinished.Broadcast();
		}
		else
		{
			SkillLockComponent->UltimateSkillFinished.Broadcast();
		}
	}

	UnbindDelegates();
}

void AFlatSkillOnly::UnbindDelegates()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AFlatSkillOnly::OnNotifyBeginReceived);
	}
}