#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/DataType.h"
#include "LoginBase.generated.h"

class AHeroBase;

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API ALoginBase : public APlayerController
{
	GENERATED_BODY()

public:
	ALoginBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	//virtual void Destroyed() override;
	
private:
	void GetValueFromSaveGame();

private:
	UPROPERTY()
	UUserWidget* Login;

	UPROPERTY()
	TSubclassOf<AHeroBase> DefaultHeroType;

	UPROPERTY()
	TSubclassOf<AHeroBase> CurrentHeroType;

	UPROPERTY()
	FString SaveGameSlotName;

	UPROPERTY()
	EGameLevelName DefaultGameModeType;

	UPROPERTY()
	EGameLevelName CurrentGameModeType;

	UPROPERTY()
	ESessionType SessionType;

	UPROPERTY()
	ESessionExtraSetting ExtraSettingsKeys;
};
