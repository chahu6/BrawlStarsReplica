// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GetActorScreenPointComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAWLSTARS_API UGetActorScreenPointComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGetActorScreenPointComponent();

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	
	bool GetActorScreenPoint(FVector2D& GetActorScreenPoint, FVector2D& GetActorScreenMousePoint);
};
