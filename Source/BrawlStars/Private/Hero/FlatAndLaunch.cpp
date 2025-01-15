#include "Hero/FlatAndLaunch.h"
#include "Hero/HeroType/AimingFlat.h"
#include "Components/SkillLockComponent.h"
#include "Hero/HeroType/AimingLaunch.h"

void AFlatAndLaunch::BeginPlay()
{
	Super::BeginPlay();

	EquipFlatAimingManager();
	EquipLaunchAimingManager();
}

void AFlatAndLaunch::ActiveNormalSkill()
{
	if (AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = true;
	AimingManager.FlatAimingManager->CenterMousePosition();
}

void AFlatAndLaunch::ReleaseNormalSkill()
{
	if (AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = false;
	NormalFlatSkill();
}

void AFlatAndLaunch::ActiveUltimateSkill()
{
	if (AimingManager.LaunchAimingManager == nullptr) return;

	AimingManager.LaunchAimingManager->AimingInfo.bIsLaunchAming = true;
	AimingManager.LaunchAimingManager->CenterMousePosition();
}

void AFlatAndLaunch::ReleaseUltimateSkill()
{
	if (AimingManager.LaunchAimingManager == nullptr) return;

	AimingManager.LaunchAimingManager->AimingInfo.bIsLaunchAming = false;
	UltimateLaunchSkill();
}

void AFlatAndLaunch::NormalFlatSkill()
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr || AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->SkillMontageStop();
	AimingManager.FlatAimingManager->SetHeroSkillReleaseRotation(HeroSkillMontage.NormalReleaseOffset);
	AimingManager.FlatAimingManager->LockMovementOrientRotation();
	AimingManager.FlatAimingManager->PlayMontage(HeroSkillMontage.NormalMontage, 1.0f, HeroSkillMontage.NormalMontageSection);

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &AFlatAndLaunch::OnMontageEnded);

	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, HeroSkillMontage.NormalMontage);
	if (!GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Contains(this, "OnNotifyBeginReceived"))
	{
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AFlatAndLaunch::OnNotifyBeginReceived);
	}
}

void AFlatAndLaunch::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (!HasAuthority() || AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->ReleaseFlatSkill(HeroSkillMontage.NormalSkillClass);
}

void AFlatAndLaunch::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		AimingManager.FlatAimingManager->UnlockMovementOrientRotation();
		if (SkillLockComponent)
		{
			SkillLockComponent->NormalSkillFinished.Broadcast();
		}
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AFlatAndLaunch::OnNotifyBeginReceived);
	}
}
