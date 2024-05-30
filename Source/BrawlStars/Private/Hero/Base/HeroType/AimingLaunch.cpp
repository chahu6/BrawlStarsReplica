#include "Hero/Base/HeroType/AimingLaunch.h"

AAimingLaunch::AAimingLaunch()
{
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;

}

void AAimingLaunch::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAimingLaunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

