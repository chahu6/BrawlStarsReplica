#include "Hero/Shelly/ShellyHero.h"

AShellyHero::AShellyHero()
{
}

void AShellyHero::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Hero/Shelly/Asset/Body/SK_Shelly_Body.SK_Shelly_Body'"));
	if (SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh);
	}

	// 要在后面加个_C
	UClass* AnimInstanceClass = LoadClass<UAnimInstance>(nullptr, TEXT("/Script/Engine.AnimBlueprint'/Game/Hero/Shelly/Animation/ABP_Shelly.ABP_Shelly_C'"));
	if (AnimInstanceClass)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	}
}
