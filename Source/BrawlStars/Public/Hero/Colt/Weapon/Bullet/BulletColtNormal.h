#pragma once

#include "CoreMinimal.h"
#include "Hero/Base/Weapon/Bullet/BulletBase.h"
#include "BulletColtNormal.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API ABulletColtNormal : public ABulletBase
{
	GENERATED_BODY()
public:
	ABulletColtNormal();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Bullet;
};
