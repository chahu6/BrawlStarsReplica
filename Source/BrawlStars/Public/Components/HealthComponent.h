#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

class UDamageTextComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BRAWLSTARS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AHeroBase;
public:	
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnHealthChanged OnHealthChanged;
	FOnHealthChanged OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MultiResetRestedTime();

	UFUNCTION(NetMulticast, Reliable)
	void MultiShowDamage(float Damage);

	UFUNCTION()
	void OnRecevieDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;
	UFUNCTION()
	void OnRep_CurrentHealth(float LastHealth);

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	float RestedTime;

	UPROPERTY()
	bool bIsDead;

public:
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
};
