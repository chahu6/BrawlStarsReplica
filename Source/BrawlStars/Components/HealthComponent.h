#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAWLSTARS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AHeroBase;
public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MultiResetRestedTime();

	UFUNCTION(NetMulticast, Reliable)
	void MultiShowDamage(float Damage);

private:
	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	float RestedTime;
};
