#include "Hero/Base/Skill/SkillBase.h"
#include "Engine/DataTable.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Hero/Base/DataInfo/DataInfo.h"
#include "Hero/Base/Weapon/Bullet/BulletBase.h"
#include "BrawlStars/BrawlStars.h"

ASkillBase::ASkillBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
}

void ASkillBase::BeginPlay()
{
	Super::BeginPlay();
	TArray<FName> RowNames;
	
	// 第一个参数一般传入nullptr或GetTransientPackage()
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, DT_Skill);
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
	float SectorAngleSplit = SectorAngle / BulletCount;
	FRotator Rotation = GetActorRotation();
	FVector Location = GetActorLocation();
	Rotation.Yaw -= SectorAngle / 2.0f;

	for (int32 i = 0; i < BulletCount; ++i)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = GetInstigator();
		GetWorld()->SpawnActor<ABulletBase>(BulletType, Location, Rotation, SpawnParameters);
		Rotation.Yaw += SectorAngleSplit;
	}

	Destroy();
}
