#pragma once

#include "CoreMinimal.h"
#include "BrawlStars/Hero/Base/Weapon/WeaponBase.h"
#include "ColtGun.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API AColtGun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AColtGun();

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponR;
};
