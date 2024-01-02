#include "AimingFlat.h"
#include "Components/DecalComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "../DataInfo/DataInfo.h"
#include "Kismet/GameplayStatics.h"
#include "../HeroBase.h"

AAimingFlat::AAimingFlat()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	AimingDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("AimingDecal"));
	AimingDecal->SetupAttachment(RootComponent);
}

void AAimingFlat::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Hero/Base/Data/DT_Skill.DT_Skill'"));
	TArray<FName> RowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DataTable, RowNames);
	for (auto& Elem : RowNames)
	{
		FSkillAttribute* SkillInfo = DataTable->FindRow<FSkillAttribute>(Elem, TEXT("AAimingFlat"));
		AActor* OwnerCharacter = GetOwner();
		if (OwnerCharacter)
		{
			if (OwnerCharacter->GetClass() == SkillInfo->HeroType)
			{
				if (SkillInfo->SkillClassification == ESkillClassification::ESC_Normal)
				{
					DecalAngleNormal = SkillInfo->SkillReleaseAngle;
					DecalDistanceNormal = SkillInfo->BulletMoveSpeed * SkillInfo->BulletLifeSpan;
				}
				else
				{
					DecalAngleUltimate = SkillInfo->SkillReleaseAngle;
					DecalDistanceUltimate = SkillInfo->BulletMoveSpeed * SkillInfo->BulletLifeSpan;
				}
			}
		}
	}

	// ²ÄÖÊ
	//AimingDecal->SetDecalMaterial();
	AimingDecal->SetRelativeRotation(FRotator(90.0, 0.0, 0.0));
	AimingDecal->SetRelativeScale3D(FVector(1.0, 4.0, 4.0));
	AimingDecal->SetVisibility(false);
}

void AAimingFlat::BeginPlay()
{
	Super::BeginPlay();

	Hero = Cast<AHeroBase>(GetOwner());

	InitAimingScreenPoint();
}

void AAimingFlat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAimingFlat::InitAimingScreenPoint()
{
	bool bIsControlled = !Hero->IsBotControlled() && Hero->IsLocallyControlled();
	if (bIsControlled)
	{

	}
}

