#pragma once

#include "CoreMinimal.h"
#include "HeroBase.h"
#include "FlatSkillOnly.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API AFlatSkillOnly : public AHeroBase
{
	GENERATED_BODY()
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;
};
