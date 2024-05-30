// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/BrawlStarsWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BRAWLSTARS_API UOverlayWidgetController : public UBrawlStarsWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "WidgetController|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "WidgetController|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
};
