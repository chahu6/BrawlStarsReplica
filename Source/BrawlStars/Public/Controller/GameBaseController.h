// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/DataType.h"
#include "GameBaseController.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API AGameBaseController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void PlayerControllerInputDisable();
	void PlayerControllerInputEnable();

	void GameIntroduction();

private:
	ETeamType TeamType;

public:
	FORCEINLINE ETeamType GetTeamType() { return TeamType; }
};
