#include "HealthComponent.h"

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

void UHealthComponent::ResetRestedTime_Implementation()
{
	RestedTime = 0.0f;
}

void UHealthComponent::ShowDamage_Implementation(float Damage)
{
	//GetOwner()
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ShowDamage"));
}
