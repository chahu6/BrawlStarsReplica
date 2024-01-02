#include "ColtHero.h"

AColtHero::AColtHero()
{
	Hat = CreateDefaultSubobject<UStaticMeshComponent>("Hat");
	Hat->SetupAttachment(GetMesh());
}

void AColtHero::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Hero/Colt/Asset/Body/SK_Colt_Body.SK_Colt_Body'"));
	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
	}

	// 要在后面加个_C
	UClass* AnimInstanceClass = LoadClass<UAnimInstance>(nullptr, TEXT("/Script/Engine.AnimBlueprint'/Game/Hero/Colt/Animation/ABP_Colt.ABP_Colt_C'"));
	if (AnimInstanceClass)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	}

	// Hat帽子
	UStaticMesh* HatMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/Hero/Colt/Asset/Hat/SM_Colt_Hat.SM_Colt_Hat'"));
	if (HatMesh)
	{
		Hat->SetStaticMesh(HatMesh);
		Hat->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ColtHat"));
	}
}

void AColtHero::BeginPlay()
{
	Super::BeginPlay();
}
