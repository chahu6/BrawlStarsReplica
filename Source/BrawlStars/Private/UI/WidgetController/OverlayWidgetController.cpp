// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "Hero/HeroBase.h"
#include "Components/HealthComponent.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	if (AHeroBase* Hero = PlayerController->GetPawn<AHeroBase>())
	{
		OnHealthChanged.Broadcast(Hero->GetHealthComponent()->GetCurrentHealth());
		OnMaxHealthChanged.Broadcast(Hero->GetHealthComponent()->GetMaxHealth());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	if (AHeroBase* Hero = PlayerController->GetPawn<AHeroBase>())
	{
		Hero->GetHealthComponent()->OnHealthChanged.AddLambda(
			[this](float NewValue)
			{
				OnHealthChanged.Broadcast(NewValue);
			}
		);

		Hero->GetHealthComponent()->OnMaxHealthChanged.AddLambda(
			[this](float NewValue)
			{
				OnMaxHealthChanged.Broadcast(NewValue);
			}
		);
	}
}
