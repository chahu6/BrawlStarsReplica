#include "Hero/Colt/Weapon/ColtGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Hero/Colt/ColtHero.h"

AColtGun::AColtGun()
{
	//WeaponR = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponR"));
	//WeaponR->SetupAttachment(RootComponent);

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshL(TEXT("/Script/Engine.SkeletalMesh'/Game/Hero/Colt/Asset/Gun/SK_Colt_GunL.SK_Colt_GunL'"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshR(TEXT("/Script/Engine.SkeletalMesh'/Game/Hero/Colt/Asset/Gun/SK_Colt_GunR.SK_Colt_GunR'"));
	//if (SkeletalMeshL.Succeeded())
	//{
	//	Weapon->SetSkeletalMesh(SkeletalMeshL.Object);
	//}

	//if (SkeletalMeshR.Succeeded())
	//{
	//	WeaponR->SetSkeletalMesh(SkeletalMeshR.Object);
	//}
}

void AColtGun::BeginPlay()
{
	Super::BeginPlay();

	//AColtHero* OwnerCharacter = Cast<AColtHero>(GetOwner());
	//if (OwnerCharacter)
	//{
	//	WeaponR->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponR"));
	//}
}
