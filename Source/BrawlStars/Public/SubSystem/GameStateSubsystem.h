#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameStateSubsystem.generated.h"

class AHeroBase;

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API UGameStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGameStateSubsystem();
	
public:
	UPROPERTY()
	TSubclassOf<AHeroBase> HeroTypeInstance;

	UPROPERTY()
	FString PlayerNameInstance;

	UPROPERTY()
	float GameMusicVolume;

	UPROPERTY()
	float GameIntroTime;

	UPROPERTY()
	int32 PlayerAmount;
};
