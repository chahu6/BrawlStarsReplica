#include "SkillLockComponent.h"

USkillLockComponent::USkillLockComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USkillLockComponent::BeginPlay()
{
	Super::BeginPlay();

	NormalSkillCurrent = NormalSkillMax;

	GetWorld()->GetTimerManager().SetTimer(NormalSkillRechageTimer, this, &USkillLockComponent::NormalBulletRechage, 0.1f, true);
	UltimateRechage.AddUObject(this, &USkillLockComponent::RechageUltimateSkill);
	NormalSkillFinished.AddUObject(this, &USkillLockComponent::ResetNormalLock);
	UltimateSkillFinished.AddUObject(this, &USkillLockComponent::ResetUltimateLock);
}

void USkillLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNormalSkillReady();

	RechageNormalSkill();

	CheckUltimateSkillReady();
}

bool USkillLockComponent::CheckPhysicsBottonNormal(bool bPress)
{
	if (bPress)
	{
		if (bNormalButtonReleased)
		{
			if (SkillState.bIsNormalEnd && SkillState.bIsUltimateEnd)
			{
				bNormalButtonPressed = true;
				bNormalButtonReleased = false;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		if (bNormalButtonPressed)
		{
			bNormalButtonPressed = false;
			bNormalButtonReleased = true;
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool USkillLockComponent::CheckActivatableNormal()
{
	if (SkillState.bIsSkillPressEnable && SkillState.bIsNormalReady && SkillState.bIsNormalEnd && SkillState.bIsUltimateEnd)
	{
		SkillState.bIsNormalActivated = true;
		if (GetWorld()->GetNetMode() == NM_Client)
		{
			ServerSetSkillReleasable(SkillState);
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool USkillLockComponent::CheckReleaseableNormal()
{
	if (SkillState.bIsSkillReleaseEnable && SkillState.bIsNormalReady && SkillState.bIsNormalActivated && SkillState.bIsNormalEnd && SkillState.bIsUltimateEnd)
	{
		NormalSkillCurrent -= 1.0f;
		SkillState.bIsNormalActivated = false;
		SkillState.bIsNormalEnd = false;
		
		if (GetWorld()->GetNetMode() == NM_Client)
		{
			ServerSetSkillReleasable(SkillState);
			ServerSetSkillAmountCurrent(NormalSkillCurrent);
		}
	}
	else
	{
		return false;
	}

	return true;
}

void USkillLockComponent::RechageUltimateSkill()
{
	if (UltimateEnergyCurrent != UltimateEnergyDefault)
	{
		UltimateEnergyCurrent = FMath::Clamp(UltimateEnergyCurrent + 1.0f, 0.0, UltimateEnergyDefault);
	}
}

void USkillLockComponent::ResetNormalLock()
{
	SkillState.bIsNormalEnd = true;
	if (!GetOwner()->HasAuthority())
	{

	}
}

void USkillLockComponent::ResetUltimateLock()
{
	SkillState.bIsSkillPressEnable = true;
	SkillState.bIsUltimateEnd = true;
	if (!GetOwner()->HasAuthority())
	{

	}
}

void USkillLockComponent::NormalBulletRechage()
{
	float Value = NormalSkillCurrent + (0.1f / NormalSKillRechageTime);
	NormalSkillCurrent = FMath::Clamp(Value, 0.0f, NormalSkillMax);
}

void USkillLockComponent::CheckNormalSkillReady()
{
	if (NormalSkillCurrent >= 1.0f)
	{
		SkillState.bIsNormalReady = true;
	}
	else
	{
		SkillState.bIsNormalReady = false;
	}
}

void USkillLockComponent::RechageNormalSkill()
{
	bool bIsRechage = (NormalSkillCurrent != NormalSkillMax) && SkillState.bIsNormalEnd && SkillState.bIsUltimateEnd;
	if (bIsRechage)
	{
		// 定时器是否暂停
		if (GetWorld()->GetTimerManager().IsTimerPaused(NormalSkillRechageTimer))
		{
			if (NormalSkillRechageTimer.IsValid())
			{
				UWorld* World = GetWorld();
				if (World)
				{
					World->GetTimerManager().UnPauseTimer(NormalSkillRechageTimer);
				}
			}
		}
	}
	else
	{
		// 是否不暂停
		if (GetWorld()->GetTimerManager().IsTimerPaused(NormalSkillRechageTimer))
		{
			if (NormalSkillRechageTimer.IsValid())
			{
				UWorld* World = GetWorld();
				if (World)
				{
					World->GetTimerManager().PauseTimer(NormalSkillRechageTimer);
				}
			}
		}
	}
}

void USkillLockComponent::CheckUltimateSkillReady()
{
	if (UltimateEnergyCurrent >= UltimateEnergyDefault)
	{
		SkillState.bIsUltimateReady = true;
	}
	else
	{
		SkillState.bIsUltimateReady = false;
	}
}

void USkillLockComponent::ServerSetSkillAmountCurrent_Implementation(float SkillAmountCurrent)
{
	NormalSkillCurrent = SkillAmountCurrent;
}

void USkillLockComponent::ServerSetSkillReleasable_Implementation(const FSkillState& SkillReleasable)
{
	SkillState = SkillReleasable;
}
