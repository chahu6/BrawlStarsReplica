#include "Hero/HeroType/AimingFlat.h"
#include "Components/DecalComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Hero/DataInfo/DataInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Hero/HeroBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkillLockComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/GetActorScreenPointComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Controller/GameBaseController.h"
#include "Weapon/WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Hero/Skill/SkillBase.h"
#include "BrawlStars/BrawlStars.h"
#include "Settings/BrawlStarsSettings.h"

AAimingFlat::AAimingFlat()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	AimingDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("AimingDecal"));
	AimingDecal->SetupAttachment(RootComponent);

	GetActorScreenPointComponent = CreateDefaultSubobject<UGetActorScreenPointComponent>(TEXT("GetActorScreenPoint"));
}

void AAimingFlat::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const UBrawlStarsSettings* Setting = GetDefault<UBrawlStarsSettings>();

	UDataTable* DataTable = Setting->SkillDataTable.LoadSynchronous();
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
	AimingDecal->SetRelativeRotation(FRotator(90.0, 0.0, 0.0));
	AimingDecal->SetRelativeScale3D(FVector(1.0, 4.0, 4.0));
	AimingDecal->SetVisibility(false);
}

void AAimingFlat::BeginPlay()
{
	Super::BeginPlay();

	Hero = Cast<AHeroBase>(GetOwner());

	Hero->ReceiveControllerChangedDelegate.AddDynamic(this, &ThisClass::InitAimingScreenPoint);
}

void AAimingFlat::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Hero->ReceiveControllerChangedDelegate.RemoveDynamic(this, &ThisClass::InitAimingScreenPoint);
}

void AAimingFlat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AAimingFlat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FlatAimingManager();
}

void AAimingFlat::FlatAimingManager()
{
	if (!IsValid(Hero) || !Hero->IsLocallyControlled()) return;

	if (AimingInfo.bIsFlatAiming)
	{
		UpdateDecalShap();
		UpdateAimDistanceAndRotation();

		if (NotAIControlled() && FMath::Abs(AimingInfo.AimDistance) > 1.0f)
		{
			AimingDecal->SetVisibility(true);
			AimingDecal->SetWorldRotation(FRotator(90.0, AimingInfo.AimRotationYaw, 180.0));

			DoOnce.Reset();
		}
	}
	else
	{
		DoOnce.Execute([this]() {
			AimingInfo.AimDistance = 0.0f;
			AimingDecal->SetVisibility(false);
		});
	}
}

void AAimingFlat::CenterMousePosition()
{
	const int64 X = FMath::TruncToInt(AimingInfo.ViewportCentrePoint.X);
	const int64 Y = FMath::TruncToInt(AimingInfo.ViewportCentrePoint.Y);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->IsLocalController() && NotAIControlled())
	{
		PC->SetMouseLocation(X, Y);
	}
}

void AAimingFlat::SkillMontageStop()
{
	if (IsValid(Hero))
	{
		Hero->StopAnimMontage();
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

void AAimingFlat::UnlockMovementOrientRotation()
{
	if (IsValid(Hero))
	{
		Hero->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AAimingFlat::PlayMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	if (IsValid(Hero))
	{
		Hero->PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
	}
}

void AAimingFlat::ReleaseFlatSkill(const TSubclassOf<ASkillBase>& InSkillClass)
{
	if (IsValid(Hero))
	{
		AWeaponBase* Weapon = Hero->GetWeapon();
		if (IsValid(Weapon))
		{
			const FVector SocketLocation = Weapon->GetRootComponent()->GetSocketLocation(FName("Muzzle"));

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParameters.Instigator = Hero;
			SpawnParameters.Owner = GetOwner();
			GetWorld()->SpawnActor<ASkillBase>(InSkillClass, SocketLocation, Hero->GetCapsuleComponent()->GetForwardVector().ToOrientationRotator(), SpawnParameters);
		}
	}
}

void AAimingFlat::InitAimingScreenPoint(APawn* Pawn, AController* OldController, AController* NewController)
{
	const bool bIsControlled = !Hero->IsBotControlled() && Hero->IsLocallyControlled();
	if (bIsControlled)
	{
		FLatentActionInfo LatentInfo(0, FMath::Rand(), TEXT("ViewportFinish"), this); // 等待Viewport初始化完成，不然位置就有问题
		UKismetSystemLibrary::Delay(this, 0.2f, LatentInfo);
	}
}

bool AAimingFlat::NotAIControlled()
{
	Hero = Hero == nullptr ? Cast<AHeroBase>(GetOwner()) : Hero;
	if (Hero)
	{
		return !Hero->IsBotControlled();
	}

	return false;
}

void AAimingFlat::UpdateDecalShap()
{
	USkillLockComponent* SkillLockComponent = Hero->GetSkillLockComponent();
	if (SkillLockComponent)
	{
		float Angle = 0.f;
		float Distance = 0.f;

		if (/*SkillLockComponent->SkillState.bIsNormalActivated*/true)
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
			if (Decal_Rectangle)
			{
				AimingDecal->SetDecalMaterial(Decal_Rectangle);

				check(Aim_Decal);
				UKismetMaterialLibrary::SetScalarParameterValue(this, Aim_Decal, TEXT("Angle"), 0.0f);
				UKismetMaterialLibrary::SetScalarParameterValue(this, Aim_Decal, TEXT("Distance"), 1.0f - (Distance / 1400.0f) - 0.3f);
			}
		}
		else
		{
			if (Decal_Sector)
			{
				AimingDecal->SetDecalMaterial(Decal_Sector);

				check(Aim_Decal);
				UKismetMaterialLibrary::SetScalarParameterValue(this, Aim_Decal, TEXT("Angle"), Angle);
				UKismetMaterialLibrary::SetScalarParameterValue(this, Aim_Decal, TEXT("Distance"), (Distance / 1500.0f) + 0.3f);
			}
		}
	}
}

void AAimingFlat::UpdateAimDistanceAndRotation()
{
	if (NotAIControlled())
	{
		const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
		if (!AimingInfo.RealViewportCenterMousePoint.Equals(MousePosition, 0.0001))
		{
			const FVector2D ToDirection = (AimingInfo.RealViewportCenterMousePoint - MousePosition);
			const float NewAimDistance = ToDirection.Size();
			float NewAimRotationZ = UKismetMathLibrary::DegAtan2(ToDirection.X * -1.f, ToDirection.Y);

			GEngine->AddOnScreenDebugMessage(12, 5.f, FColor::Green, FString::Printf(TEXT("AimRotationZ = %f"), NewAimRotationZ));

			AGameBaseController* GameBase = Cast<AGameBaseController>(UGameplayStatics::GetPlayerController(this, 0));
			if (GameBase)
			{
				if (GameBase->GetTeamType() == ETeamType::ET_TeamB)
				{
					NewAimRotationZ += 180.0f;
				}
			}

			const float LastAimRotationZ = AimingInfo.AimRotationYaw;
			if (FMath::Abs(LastAimRotationZ - NewAimRotationZ) < 100.0f)
			{
				NewAimRotationZ = FMath::FInterpTo(LastAimRotationZ, NewAimRotationZ, UGameplayStatics::GetWorldDeltaSeconds(this), 40.0f);
			}

			AimingInfo.AimDistance = NewAimDistance;
			AimingInfo.AimRotationYaw = NewAimRotationZ;
			//UE_LOG(LogTemp, Warning, TEXT("Distance: %f, RotationZ: %f"), NewAimDistance, NewAimRotationZ);
			if (!HasAuthority())
			{
				// 。。。。。。
			}
		}
	}
}

void AAimingFlat::InitActorScreenPoint()
{
	FVector2D ActorScreenPoint, ActorScreenMousePoint;
	if (GetActorScreenPointComponent->GetActorScreenPoint(ActorScreenPoint, ActorScreenMousePoint))
	{
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

