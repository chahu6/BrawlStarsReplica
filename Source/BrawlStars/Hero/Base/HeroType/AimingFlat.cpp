#include "AimingFlat.h"
#include "Components/DecalComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "../DataInfo/DataInfo.h"
#include "Kismet/GameplayStatics.h"
#include "../HeroBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrawlStars/Components/SkillLockComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "BrawlStars/Components/GetActorScreenPointComponent.h"

AAimingFlat::AAimingFlat()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	AimingDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("AimingDecal"));
	AimingDecal->SetupAttachment(RootComponent);

	GetActorScreenPointComponent = CreateDefaultSubobject<UGetActorScreenPointComponent>(TEXT("GetActorScreenPoint"));
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

	// 材质
	UMaterialInterface* DecalMaterial = LoadObject<UMaterialInterface>(this, TEXT("/Script/Engine.Material'/Game/Hero/Base/Asset/M_Decal_FlatAim_Rectangle.M_Decal_FlatAim_Rectangle'"));
	if (DecalMaterial)
	{
		AimingDecal->SetDecalMaterial(DecalMaterial);	
	}
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

void AAimingFlat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAimingFlat, AimingInfo);
}

void AAimingFlat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Hero))
	{
		FlatAimingManager();
	}
}

void AAimingFlat::CenterMousePosition()
{
	if (NotAIControlled())
	{
		int64 X = FMath::TruncToInt(AimingInfo.ViewportCentrePoint.X);
		int64 Y = FMath::TruncToInt(AimingInfo.ViewportCentrePoint.Y);
		UGameplayStatics::GetPlayerController(this, 0)->SetMouseLocation(X, Y);
	}
}

void AAimingFlat::SkillMontageStop()
{
	if (IsValid(Hero))
	{
		Hero->GetMesh()->GetAnimInstance()->Montage_Stop(0.0f);
	}
}

void AAimingFlat::SetHeroSkillReleaseRotation(float AddModify)
{
	float Yaw = AimingInfo.AimRotationYaw + AddModify;
	if (IsValid(Hero))
	{
		Hero->SetActorRotation(FRotator(0.0, Yaw, 0.0));
	}
}

void AAimingFlat::LockMovementOrientRotation()
{
	if (IsValid(Hero))
	{
		Hero->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AAimingFlat::ClientPlayMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	if (IsValid(Hero))
	{
		Hero->PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
	}
}

void AAimingFlat::InitAimingScreenPoint()
{
	bool bIsControlled = !Hero->IsBotControlled() && Hero->IsLocallyControlled();
	if (bIsControlled)
	{
		FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("ViewportFinish"), this); // 等待Viewport初始化完成，不然位置就有问题
		UKismetSystemLibrary::Delay(this, 0.2f, LatentInfo);
	}
}

bool AAimingFlat::NotAIControlled()
{
	AActor* MyOwner = GetOwner();
	if (IsValid(MyOwner))
	{
		APawn* Instig = MyOwner->GetInstigator();
		if (Instig)
		{
			return !Instig->IsBotControlled();
		}
	}

	return false;
}

void AAimingFlat::FlatAimingManager()
{
	if (Hero->IsLocallyControlled())
	{
		if (AimingInfo.bIsFlatAiming)
		{
			UpdateDecalShap();
		}
	}
}

void AAimingFlat::UpdateDecalShap()
{
	USkillLockComponent* SkillLockComponent = Hero->GetComponentByClass<USkillLockComponent>();
	if (SkillLockComponent)
	{
		float Angle;
		float Distance;

		if (SkillLockComponent->SkillState.bIsNormalActivated)
		{
			Angle = DecalAngleNormal / 360.0f;
			Distance = DecalDistanceNormal;
		}
		else
		{
			Angle = DecalDistanceUltimate / 360.0f;
			Distance = DecalDistanceUltimate;
		}

		if (DecalAngleNormal == 0.0f)
		{
			UMaterialInterface* Decal_Rectangle = LoadObject<UMaterialInterface>(this, TEXT("/Script/Engine.Material'/Game/Hero/Base/Asset/M_Decal_FlatAim_Rectangle.M_Decal_FlatAim_Rectangle'"));
			if (Decal_Rectangle)
			{
				AimingDecal->SetDecalMaterial(Decal_Rectangle);
				UMaterialParameterCollection* Aim_Decal = LoadObject<UMaterialParameterCollection>(this, TEXT("/Script/Engine.MaterialParameterCollection'/Game/Hero/Base/Asset/MPC_Aim_Decal.MPC_Aim_Decal'"));
				if (Aim_Decal)
				{
					UKismetMaterialLibrary::SetScalarParameterValue(this, Aim_Decal, TEXT("Angle"), 0.0f);
					UKismetMaterialLibrary::SetScalarParameterValue(this, Aim_Decal, TEXT("Distance"), 1.0f - (Distance / 1400.0f) - 0.3f);
				}
			}
		}
		else
		{
			UMaterialInterface* Decal_Sector = LoadObject<UMaterialInterface>(this, TEXT("/Script/Engine.Material'/Game/Hero/Base/Asset/M_Decal_FlatAim_Sector.M_Decal_FlatAim_Sector'"));
			if (Decal_Sector)
			{
				AimingDecal->SetDecalMaterial(Decal_Sector);
				UMaterialParameterCollection* Aim_Decal = LoadObject<UMaterialParameterCollection>(this, TEXT("/Script/Engine.MaterialParameterCollection'/Game/Hero/Base/Asset/MPC_Aim_Decal.MPC_Aim_Decal'"));
				if (Aim_Decal)
				{
					UKismetMaterialLibrary::SetScalarParameterValue(this, Aim_Decal, TEXT("Angle"), Angle);
					UKismetMaterialLibrary::SetScalarParameterValue(this, Aim_Decal, TEXT("Distance"), Distance / 1500.0f + 0.3f);
				}
			}
		}
	}
}

void AAimingFlat::InitActorScreenPoint()
{
	FVector2D ActorScreenPoint, ActorScreenMousePoint;
	if (GetActorScreenPointComponent->GetActorScreenPoint(ActorScreenPoint, ActorScreenMousePoint))
	{
		GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Red, FString::Printf(TEXT("Actor: %s, Mouse: %s"), *ActorScreenPoint.ToString(), *ActorScreenMousePoint.ToString()));

		AimingInfo.bIsViewportCentrePointSet = true;
		AimingInfo.ViewportCentrePoint = ActorScreenPoint;
		AimingInfo.RealViewportCenterMousePoint = ActorScreenMousePoint;
	}
}

void AAimingFlat::ViewportFinish()
{
	if (!AimingInfo.bIsViewportCentrePointSet)
	{
		InitActorScreenPoint();
	}
}

