#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAWLSTARS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AHeroBase;
public:	
	UHealthComponent();

	FOnHealthChanged OnHealthChanged;

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

public:
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
};
