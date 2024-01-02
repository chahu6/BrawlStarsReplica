#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBase.generated.h"

UCLASS()
class BRAWLSTARS_API ASkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
