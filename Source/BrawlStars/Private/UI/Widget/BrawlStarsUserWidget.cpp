// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BrawlStarsUserWidget.h"

void UBrawlStarsUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}