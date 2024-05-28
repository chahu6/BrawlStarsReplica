#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../Data/DataType.h"
#include "GameBase.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API AGameBase : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	ETeamType TeamType;
};
