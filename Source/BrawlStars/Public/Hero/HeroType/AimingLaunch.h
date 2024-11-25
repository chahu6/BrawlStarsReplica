#pragma once

#include "CoreMinimal.h"
#include "Hero/HeroType/AimingFlat.h"
#include "AimingLaunch.generated.h"

UCLASS()
class BRAWLSTARS_API AAimingLaunch : public AAimingFlat
{
	GENERATED_BODY()
	
public:	
	AAimingLaunch();

	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	void ReleaseLaunchSkill(AActor* ActorLaunch);

};
