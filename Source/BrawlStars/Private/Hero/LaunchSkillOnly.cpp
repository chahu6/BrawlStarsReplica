#include "Hero/LaunchSkillOnly.h"
#include "Hero/HeroType/AimingLaunch.h"
#include "Components/SkillLockComponent.h"

void ALaunchSkillOnly::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) EquipLaunchAimingManager();
}

void ALaunchSkillOnly::ActiveNormalSkill()
{
	if (AimingManager.LaunchAimingManager == nullptr) return;

	AimingManager.LaunchAimingManager->AimingInfo.bIsFlatAiming = true;
	AimingManager.LaunchAimingManager->CenterMousePosition();
}

void ALaunchSkillOnly::ReleaseNormalSkill()
{
	if (AimingManager.LaunchAimingManager == nullptr) return;

	AimingManager.LaunchAimingManager->AimingInfo.bIsFlatAiming = false;

	ReleaseSkill(HeroSkillMontage.NormalReleaseOffset, true, HeroSkillMontage.NormalMontage, HeroSkillMontage.NormalMontageSection, HeroSkillMontage.NormalSkillClass);
}

void ALaunchSkillOnly::ActiveUltimateSkill()
{
	if (AimingManager.LaunchAimingManager == nullptr) return;

	AimingManager.LaunchAimingManager->AimingInfo.bIsFlatAiming = true;
	AimingManager.LaunchAimingManager->CenterMousePosition();
}

void ALaunchSkillOnly::ReleaseUltimateSkill()
{
	if (AimingManager.LaunchAimingManager == nullptr) return;

	AimingManager.LaunchAimingManager->AimingInfo.bIsFlatAiming = false;

	ReleaseSkill(HeroSkillMontage.UltimateReleaseOffset, false, HeroSkillMontage.UltimateMontage, HeroSkillMontage.UltimateMontageSection, HeroSkillMontage.UltimateSkillClass);
}

void ALaunchSkillOnly::ReleaseSkill(float InReleaseRotationOffset, bool bNormalSkill, UAnimMontage* InMontage, FName InMontageSection, TSubclassOf<ASkillBase> InSkillClass)
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr || AimingManager.LaunchAimingManager == nullptr) return;

	AimingManager.LaunchAimingManager->SkillMontageStop();
	AimingManager.LaunchAimingManager->SetHeroSkillReleaseRotation(InReleaseRotationOffset);
	AimingManager.LaunchAimingManager->LockMovementOrientRotation();
	AimingManager.LaunchAimingManager->PlayMontage(InMontage, 1.0f, InMontageSection);

	SkillClass = InSkillClass;
	bIsNormalSkill = bNormalSkill;

	MontageEndedDelegate.BindUObject(this, &ALaunchSkillOnly::OnMontageEnded);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, InMontage);
	if (!GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Contains(this, "OnNotifyBeginReceived"))
	{
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ALaunchSkillOnly::OnNotifyBeginReceived);
	}
}

void ALaunchSkillOnly::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (!HasAuthority() || AimingManager.LaunchAimingManager == nullptr) return;

	AimingManager.LaunchAimingManager->ReleaseFlatSkill(SkillClass);
}

void ALaunchSkillOnly::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		AimingManager.LaunchAimingManager->UnlockMovementOrientRotation();
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

void ALaunchSkillOnly::UnbindDelegates()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ALaunchSkillOnly::OnNotifyBeginReceived);
	}
}
