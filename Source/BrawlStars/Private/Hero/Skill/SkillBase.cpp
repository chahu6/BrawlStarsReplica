#include "Hero/Skill/SkillBase.h"
#include "Engine/DataTable.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Hero/DataInfo/DataInfo.h"
#include "Weapon/Bullet/BulletBase.h"
#include "BrawlStars/BrawlStars.h"
#include "Settings/BrawlStarsSettings.h"
#include "BrawlStarsFunctionLibrary.h"

ASkillBase::ASkillBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
}

void ASkillBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<FName> RowNames;

	const UBrawlStarsSettings* Setting = GetDefault<UBrawlStarsSettings>();
	
	// 第一个参数一般传入nullptr或GetTransientPackage()
	UDataTable* DataTable = Setting->SkillDataTable.LoadSynchronous();
	RowNames.Empty();
	UDataTableFunctionLibrary::GetDataTableRowNames(DataTable, RowNames);
	for (auto& Elem : RowNames)
	{
		FSkillAttribute* SkillAttribute = DataTable->FindRow<FSkillAttribute>(Elem, TEXT("AHeroBase"));
		if (GetClass() == SkillAttribute->SkillType)
		{
			ReleaseBullet(SkillAttribute->BulletType, SkillAttribute->SkillProjectile, SkillAttribute->SkillReleaseAngle);
			break;
		}
	}
}

void ASkillBase::ReleaseBullet(const TSubclassOf<ABulletBase>& BulletType, int32 BulletCount, float SectorAngle)
{
	const FVector Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();

	TArray<FRotator> Rotations = UBrawlStarsFunctionLibrary::EvenlySpacedRotators(Rotation.Vector(), FVector::UpVector, SectorAngle, BulletCount);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = GetOwner();

	for (const FRotator& Rot : Rotations)
	{
		GetWorld()->SpawnActor<ABulletBase>(BulletType, Location, Rot, SpawnParameters);
	}

	Destroy();
}
