#include "Weapon/Bullet/BulletBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Hero/DataInfo/DataInfo.h"
#include "Kismet/GameplayStatics.h"
#include "BrawlStars/BrawlStars.h"
#include "Hero/HeroBase.h"
#include "Components/SkillLockComponent.h"
#include "Settings/BrawlStarsSettings.h"

ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Cube = CreateDefaultSubobject<UBoxComponent>(TEXT("Cube"));
	RootComponent = Cube;
	Cube->SetCollisionProfileName("BulletOverlap");
	Cube->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetCollisionProfileName("BulletOverlap");
	Sphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	BulletHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletHead"));
	BulletHead->SetupAttachment(RootComponent);
	BulletHead->SetCollisionProfileName("NoCollision");
	BulletHead->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	ProjectilMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectilMovement"));
	ProjectilMovement->ProjectileGravityScale = 0.0f;
}

void ABulletBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const UBrawlStarsSettings* Setting = GetDefault<UBrawlStarsSettings>();

	UDataTable* DataTable = Setting->SkillDataTable.LoadSynchronous();
	TArray<FName> RowNames;
	if (DataTable)
	{
		RowNames.Empty();
		UDataTableFunctionLibrary::GetDataTableRowNames(DataTable, RowNames);
		for (auto& Elem : RowNames)
		{
			FSkillAttribute* SkillAttribute = DataTable->FindRow<FSkillAttribute>(Elem, TEXT("ABulletBase"));
			if (SkillAttribute && GetClass() == SkillAttribute->BulletType)
			{
				Damage = SkillAttribute->BulletDamage;
				ProjectilMovement->InitialSpeed = SkillAttribute->BulletMoveSpeed;
				ProjectilMovement->MaxSpeed = SkillAttribute->BulletMoveSpeed;
				SetLifeSpan(SkillAttribute->BulletLifeSpan);
				bIsLongLive = SkillAttribute->bBuletIsLongLive;
				bSenceDestructible = SkillAttribute->bBulletSenceDestructible;
				break;
			}
		}
	}

	DataTable = Setting->BulletSoundEffectDataTable.LoadSynchronous();
	if (DataTable)
	{
		RowNames.Empty();
		UDataTableFunctionLibrary::GetDataTableRowNames(DataTable, RowNames);
		for (const auto& Elem : RowNames)
		{
			FBulletSoundEffect* BulletSound = DataTable->FindRow<FBulletSoundEffect>(Elem, TEXT("ABulletBase"));
			if (BulletSound && GetClass() == BulletSound->BulletType)
			{
				BulletSoundEffect = BulletSound->BulletSoundEffect;
				BulletSoundVolume = BulletSound->BulletSoundVolume;
				break;
			}
		}
	}
}

void ABulletBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(Cube) && HasAuthority())
	{
		Cube->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCubeBeginOverlap);
	}

	if (IsValid(Sphere) && HasAuthority())
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	}
}

void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BulletSoundEffect))
	{
		//if (BulletSoundAttenuation)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BulletSoundEffect, GetActorLocation(), BulletSoundVolume, 1.0f, 0.0f, BulletSoundAttenuation);
		}
	}
}

void ABulletBase::OnCubeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;
	UE_LOG(LogTemp, Warning, TEXT("OnCubeBeginOverlap"));

	if(CheckTeamate(OtherActor))
	{
		HitEnemy();
	}
}

void ABulletBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;
	UE_LOG(LogTemp, Warning, TEXT("OnSphereBeginOverlap"));
}

bool ABulletBase::CheckTeamate(AActor* OtherActor)
{
	Enemy = Cast<AHeroBase>(OtherActor);
	return Enemy != nullptr;
}

void ABulletBase::HitEnemy()
{
	RechageUltimate();
	ApplyDamageAndDestroy();
}

void ABulletBase::RechageUltimate()
{
	AHeroBase* Attacker = Cast<AHeroBase>(GetInstigator());
	if (IsValid(Attacker) && Attacker->GetSkillLockComponent())
	{
		Attacker->GetSkillLockComponent()->UltimateRechage.Broadcast();
	}
}

void ABulletBase::ApplyDamageAndDestroy()
{
	UGameplayStatics::ApplyDamage(Enemy, Damage, nullptr, this, UDamageType::StaticClass());
	Destroy();
}
