#include "Hero/Base/Weapon/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	RootComponent = Weapon;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

