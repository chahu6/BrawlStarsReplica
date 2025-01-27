#include "Components/SkillLockComponent.h"

USkillLockComponent::USkillLockComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void USkillLockComponent::BeginPlay()
{
	Super::BeginPlay();

	NormalSkillCurrentValue = NormalSkillMaxAmount;

	GetWorld()->GetTimerManager().SetTimer(NormalSkillRechageTimer, this, &USkillLockComponent::NormalBulletRechage, 0.1f, true);

	UltimateRechage.AddDynamic(this, &USkillLockComponent::RechageUltimateSkill);
	NormalSkillFinished.AddDynamic(this, &USkillLockComponent::ResetNormalLock);
	UltimateSkillFinished.AddDynamic(this, &USkillLockComponent::ResetUltimateLock);
}

void USkillLockComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(NormalSkillRechageTimer);
}

void USkillLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNormalSkillReady();

	RechageNormalSkill();

	CheckUltimateSkillReady();
}

void USkillLockComponent::ResetUltimateEnergyCurrent()
{
	UltimateEnergyCurrent = 0.f;
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
		return true;
	}

	return false;
}

bool USkillLockComponent::CheckReleaseableNormal()
{
	if (SkillState.bIsSkillReleaseEnable && SkillState.bIsNormalReady && SkillState.bIsNormalActivated && SkillState.bIsNormalEnd && SkillState.bIsUltimateEnd)
	{
		NormalSkillCurrentValue = FMath::Clamp(NormalSkillCurrentValue - 1.f, 0, NormalSkillMaxAmount);
		SkillState.bIsNormalActivated = false;
		SkillState.bIsNormalEnd = false;
		
		if (GetWorld()->GetNetMode() == NM_Client)
		{
			ServerSetSkillReleasable(SkillState);
			ServerSetSkillAmountCurrent(NormalSkillCurrentValue);
		}
		return true;
	}

	return false;
}

bool USkillLockComponent::CheckActivatableUltimate()
{
	if (SkillState.bIsSkillPressEnable && SkillState.bIsUltimateReady && SkillState.bIsUltimateEnd)
	{
		SkillState.bIsUltimateActivated = true;
		SkillState.bIsSkillPressEnable = false;
		if (GetWorld()->GetNetMode() == NM_Client)
		{
			ServerSetSkillReleasable(SkillState);
		}
		return true;
	}
	return false;
}

bool USkillLockComponent::CheckReleaseableUltimate()
{
	if (SkillState.bIsSkillReleaseEnable && SkillState.bIsUltimateReady && SkillState.bIsUltimateActivated && SkillState.bIsUltimateEnd)
	{
		SkillState.bIsSkillPressEnable = false;
		SkillState.bIsNormalEnd = true;
		SkillState.bIsNormalActivated = false;
		SkillState.bIsUltimateEnd = false;
		if (GetWorld()->GetNetMode() == NM_Client)
		{
			ServerSetSkillReleasable(SkillState);
		}
		return true;
	}
	return false;
}

void USkillLockComponent::RechageUltimateSkill()
{
	if (UltimateEnergyCurrent != UltimateEnergyDefault)
	{
		UltimateEnergyCurrent = FMath::Clamp(UltimateEnergyCurrent + 1.0f, 0.0f, UltimateEnergyDefault);
	}
	FOnUltimateEnergyChangedDelegate.Broadcast(UltimateEnergyCurrent / UltimateEnergyDefault);
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
	if (NormalSkillCurrentValue != NormalSkillMaxAmount)
	{
		const float Value = NormalSkillCurrentValue + (0.1f / NormalSKillRechageTime);
		NormalSkillCurrentValue = FMath::Clamp(Value, 0.0f, NormalSkillMaxAmount);
	}
	NormalSkillValueChangedDelegate.Broadcast(NormalSkillCurrentValue, NormalSkillMaxAmount);
}

void USkillLockComponent::CheckNormalSkillReady()
{
	if (NormalSkillCurrentValue >= 1.0f)
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
	bool bIsRechage = (NormalSkillCurrentValue != NormalSkillMaxAmount) && SkillState.bIsNormalEnd && SkillState.bIsUltimateEnd;
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
	NormalSkillCurrentValue = SkillAmountCurrent;
}

void USkillLockComponent::ServerSetSkillReleasable_Implementation(const FSkillState& SkillReleasable)
{
	SkillState = SkillReleasable;
}
