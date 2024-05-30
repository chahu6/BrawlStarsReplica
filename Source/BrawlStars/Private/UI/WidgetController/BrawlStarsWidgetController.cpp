// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BrawlStarsWidgetController.h"

void UBrawlStarsWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
}
