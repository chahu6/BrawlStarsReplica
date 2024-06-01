#include "Components/HealthComponent.h"
#include "Hero/Base/HeroBase.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsDead = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner && Owner->HasAuthority())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnRecevieDamage);
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::OnRecevieDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamagedActor == GetOwner() && DamageCauser != GetOwner() && !bIsDead)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
		OnHealthChanged.Broadcast(CurrentHealth);

		if (CurrentHealth <= 0.0f)
		{
			bIsDead = true;

			if (AHeroBase* Hero = GetOwner<AHeroBase>())
			{
				//Hero->HeroDie();
			}
			else
			{
				if (GetOwner()) GetOwner()->Destroy();
			}
		}
	}
}

void UHealthComponent::OnRep_CurrentHealth(float LastHealth)
{
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::MultiResetRestedTime_Implementation()
{
	RestedTime = 0.0f;
}

void UHealthComponent::MultiShowDamage_Implementation(float Damage)
{
	//GetOwner()
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ShowDamage"));
}
