#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AimingLaunch.generated.h"

UCLASS()
class BRAWLSTARS_API AAimingLaunch : public AActor
{
	GENERATED_BODY()
	
public:	
	AAimingLaunch();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
