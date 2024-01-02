#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AimingFlat.generated.h"

class AHeroBase;

UCLASS()
class BRAWLSTARS_API AAimingFlat : public AActor
{
	GENERATED_BODY()
	
public:	
	AAimingFlat();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	void InitAimingScreenPoint();

private:
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* AimingDecal;

private:
	UPROPERTY()
	AHeroBase* Hero;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalAngleNormal;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalAngleUltimate;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalDistanceNormal;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalDistanceUltimate;
};
