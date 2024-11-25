#include "Hero/FlatSkillOnly.h"
#include "Hero/HeroType/AimingFlat.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkillLockComponent.h"

void AFlatSkillOnly::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AFlatSkillOnly::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority()) EquipFlatAimingManager();
}

void AFlatSkillOnly::ActiveNormalSkill()
{
	if (AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = true;
	AimingManager.FlatAimingManager->CenterMousePosition();
}

void AFlatSkillOnly::ReleaseNormalSkill()
{
	if (AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = false;

	ReleaseSkill(HeroSkillMontage.NormalReleaseOffset, true, HeroSkillMontage.NormalMontage, HeroSkillMontage.NormalMontageSection, HeroSkillMontage.NormalSkillClass);
	ServerReleaseSkill(AimingManager.FlatAimingManager->AimingInfo, HeroSkillMontage.NormalReleaseOffset, true, HeroSkillMontage.NormalMontage, HeroSkillMontage.NormalMontageSection, HeroSkillMontage.NormalSkillClass);
}

void AFlatSkillOnly::ActiveUltimateSkill()
{
	if (AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = true;
	AimingManager.FlatAimingManager->CenterMousePosition();
}

void AFlatSkillOnly::ReleaseUltimateSkill()
{
	if (AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->AimingInfo.bIsFlatAiming = false;

	ReleaseSkill(HeroSkillMontage.UltimateReleaseOffset, false, HeroSkillMontage.UltimateMontage, HeroSkillMontage.UltimateMontageSection, HeroSkillMontage.UltimateSkillClass);
}

void AFlatSkillOnly::ServerReleaseSkill_Implementation(const FAimInfo& AimInfo, float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> InSkillClass)
{
	MulticastReleaseSkill(AimInfo, ReleaseRotationOffset, bNormalSkill, Montage, MontageSection, InSkillClass);
}

void AFlatSkillOnly::MulticastReleaseSkill_Implementation(const FAimInfo& AimInfo, float ReleaseRotationOffset, bool bNormalSkill, UAnimMontage* Montage, FName MontageSection, TSubclassOf<ASkillBase> InSkillClass)
{
	if (IsLocallyControlled()) return;

	AimingManager.FlatAimingManager->AimingInfo = AimInfo;
	ReleaseSkill(ReleaseRotationOffset, bNormalSkill, Montage, MontageSection, InSkillClass);
}

void AFlatSkillOnly::ReleaseSkill(float InReleaseRotationOffset, bool bNormalSkill, UAnimMontage* InMontage, FName InMontageSection, TSubclassOf<ASkillBase> InSkillClass)
{
	if (GetMesh() == nullptr || GetMesh()->GetAnimInstance() == nullptr || AimingManager.FlatAimingManager == nullptr) return;

	AimingManager.FlatAimingManager->SkillMontageStop();
	AimingManager.FlatAimingManager->SetHeroSkillReleaseRotation(InReleaseRotationOffset);
	AimingManager.FlatAimingManager->LockMovementOrientRotation();
	AimingManager.FlatAimingManager->PlayMontage(InMontage, 1.0f, InMontageSection);

	SkillClass = InSkillClass;
	bIsNormalSkill = bNormalSkill;

	MontageEndedDelegate.BindUObject(this, &AFlatSkillOnly::OnMontageEnded);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, InMontage);
	if (!GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.Contains(this, "OnNotifyBeginReceived"))
	{
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AFlatSkillOnly::OnNotifyBeginReceived);
	}
}

void AFlatSkillOnly::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (!HasAuthority() || AimingManager.FlatAimingManager == nullptr) return;

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