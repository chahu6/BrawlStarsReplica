#pragma once

#include "CoreMinimal.h"
#include "BrawlStars/Hero/Base/FlatSkillOnly.h"
#include "ColtHero.generated.h"

UCLASS()
class BRAWLSTARS_API AColtHero : public AFlatSkillOnly
{
	GENERATED_BODY()
public:
	AColtHero();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Hat;
};
