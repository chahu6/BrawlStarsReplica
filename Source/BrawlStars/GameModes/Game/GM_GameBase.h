// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_GameBase.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API AGM_GameBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
};
