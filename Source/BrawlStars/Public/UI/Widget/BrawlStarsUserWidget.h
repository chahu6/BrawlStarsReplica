// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrawlStarsUserWidget.generated.h"

class AHeroBase;

/**
 * 
 */
UCLASS()
class BRAWLSTARS_API UBrawlStarsUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AHeroBase> Hero;
};
