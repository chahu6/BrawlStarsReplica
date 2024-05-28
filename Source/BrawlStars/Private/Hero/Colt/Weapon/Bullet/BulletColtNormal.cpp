#include "Hero/Colt/Weapon/Bullet/BulletColtNormal.h"
#include "Components/BoxComponent.h"

ABulletColtNormal::ABulletColtNormal()
{
	Bullet = CreateDefaultSubobject<UStaticMeshComponent>("Bullet");
	Bullet->SetupAttachment(GetRootComponent());
}

void ABulletColtNormal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}
