#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBase.generated.h"

class ABulletBase;

UCLASS()
class BRAWLSTARS_API ASkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillBase();

protected:
	virtual void BeginPlay() override;

	void ReleaseBullet(const TSubclassOf<ABulletBase>& BulletType, int32 BulletCount, float SectorAngle);
};
