// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BrawlStarsHUD.h"
#include "UI/Widget/BrawlStarsUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void ABrawlStarsHUD::InitOverlay(APlayerController* PC, APlayerState* PS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized"));

	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(PlayerController, OverlayWidgetClass);
		OverlayWidget = Cast<UBrawlStarsUserWidget>(Widget);
		check(OverlayWidget);

		const FWidgetControllerParams WCParams(PC, PS);
		UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WCParams);
		check(WidgetController);

		OverlayWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValue();

		OverlayWidget->AddToViewport();
	}
}

UOverlayWidgetController* ABrawlStarsHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}
