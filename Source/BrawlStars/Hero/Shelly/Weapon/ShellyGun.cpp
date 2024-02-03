#include "ShellyGun.h"

AShellyGun::AShellyGun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Hero/Shelly/Asset/Gun/SK_Shelly_Gun.SK_Shelly_Gun'"));
	if (SkeletalMesh.Succeeded())
	{
		Weapon->SetSkeletalMesh(SkeletalMesh.Object);
	}
}

void AShellyGun::BeginPlay()
{
	Super::BeginPlay();


}
