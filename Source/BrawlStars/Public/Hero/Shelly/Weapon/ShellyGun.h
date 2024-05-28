#pragma once

#include "CoreMinimal.h"
#include "Hero/Base/Weapon/WeaponBase.h"
#include "ShellyGun.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API AShellyGun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AShellyGun();

	virtual void BeginPlay() override;
};
