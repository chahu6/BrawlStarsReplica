// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BrawlStarsHUD.generated.h"

class UBrawlStarsUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class BRAWLSTARS_API ABrawlStarsHUD : public AHUD
{
	GENERATED_BODY()
public:
	void InitOverlay(APlayerController* PC, APlayerState* PS);

protected:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBrawlStarsUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UBrawlStarsUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
};
