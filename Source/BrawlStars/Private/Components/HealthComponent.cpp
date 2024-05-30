#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

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
