#include "Hero/HeroType/AimingLaunch.h"
#include "Hero/HeroBase.h"

AAimingLaunch::AAimingLaunch()
{
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;

}

void AAimingLaunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Hero) && Hero->IsLocallyControlled() && AimingInfo.bIsLaunchAming)
	{
		UpdateAimDistanceAndRotation();

		if (AimingInfo.AimDistance != 0.f)
		{

		}
	}
	else
	{

	}
}

void AAimingLaunch::ReleaseLaunchSkill(AActor* ActorLaunch)
{

}

