#include "Hero/HeroBase.h"
#include "Components/HealthComponent.h"
#include "Components/SkillLockComponent.h"
#include "Hero/HeroType/AimingFlat.h"
#include "Hero/HeroType/AimingLaunch.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Weapon/WeaponBase.h"
#include "Components/WidgetComponent.h"
#include "PlayerState/GameBase.h"
#include "Net/UnrealNetwork.h"
#include "BrawlStars/BrawlStars.h"
#include "UI/HUD/BrawlStarsHUD.h"
#include "Settings/BrawlStarsSettings.h"
#include "UI/Widget/BrawlStarsUserWidget.h"

AHeroBase::AHeroBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetEnableGravity(false);

	TeamDecal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeamDecal"));
	TeamDecal->SetupAttachment(RootComponent);
	TeamDecal->SetRelativeLocation(TeamDecal->GetComponentLocation() - FVector(0, 0, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 1000.0f;
	CameraBoom->SetRelativeRotation(FRotator(310.0, 0.0, 0.0));
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
	GetMesh()->SetRelativeScale3D(FVector(0.14));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->SetIsReplicated(true);

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetupAttachment(RootComponent);
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	SkillLockComponent = CreateDefaultSubobject<USkillLockComponent>(TEXT("SkillLock"));
	SkillLockComponent->SetIsReplicated(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0, 720.0, 0.0);
}

void AHeroBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TeamDecal->SetHiddenInGame(true);
	TArray<FName> RowNames;

	const UBrawlStarsSettings* Setting = GetDefault<UBrawlStarsSettings>();

	// 英雄初始化
	{
		// 第一个参数一般传入nullptr或GetTransientPackage()
		UDataTable* DataTable = Setting->HeroDataTable.LoadSynchronous();
		RowNames.Empty();
		UDataTableFunctionLibrary::GetDataTableRowNames(DataTable, RowNames);
		for (auto& Elem : RowNames)
		{
			FHeroAttribute* HeroInfo = DataTable->FindRow<FHeroAttribute>(Elem, TEXT("AHeroBase"));
			if (GetClass() == HeroInfo->HeroType)
			{
				WeaponClass = HeroInfo->WeaponType;
				HealthComponent->CurrentHealth = HeroInfo->DefaultHealth;
				HealthComponent->MaxHealth = HeroInfo->DefaultHealth;
				GetCharacterMovement()->MaxWalkSpeed = HeroInfo->MoveSpeed;
				break;
			}
		}
	}

	// 技能初始化
	{
		UDataTable* DataTable = Setting->SkillDataTable.LoadSynchronous();
		RowNames.Empty();
		UDataTableFunctionLibrary::GetDataTableRowNames(DataTable, RowNames);
		for (auto& Elem : RowNames)
		{
			FSkillAttribute* SkillInfo = DataTable->FindRow<FSkillAttribute>(Elem, TEXT("AHeroBase"));
			if (GetClass() == SkillInfo->HeroType)
			{
				if (SkillInfo->SkillClassification == ESkillClassification::ESC_Normal)
				{
					SkillLockComponent->NormalSKillRechageTime = SkillInfo->SkillRechage;
					SkillLockComponent->NormalSkillMax = SkillInfo->BulletAmmo;
				}
				else
				{
					SkillLockComponent->UltimateEnergyDefault = SkillInfo->SkillRechage;
				}
			}
		}
	}

	// 技能蒙太奇
	{
		UDataTable* DataTable = Setting->SkillMontageDataTable.LoadSynchronous();
		RowNames.Empty();
		UDataTableFunctionLibrary::GetDataTableRowNames(DataTable, RowNames);
		for (auto& Elem : RowNames)
		{
			FSkillMontage* SkillMontage = DataTable->FindRow<FSkillMontage>(Elem, TEXT("AHeroBase"));
			if (GetClass() == SkillMontage->HeroType)
			{
				HeroSkillMontage = *SkillMontage;
				break;
			}
		}
	}

	// 英雄台词
	{
		UDataTable* DataTable = Setting->HeroSpeakLineDataTable.LoadSynchronous();
		RowNames.Empty();
		UDataTableFunctionLibrary::GetDataTableRowNames(DataTable, RowNames);
		for (auto& Elem : RowNames)
		{
			FHeroSpeakLine* HeroSpeakLine = DataTable->FindRow<FHeroSpeakLine>(Elem, TEXT("AHeroBase"));
			if (GetClass() == HeroSpeakLine->HeroType)
			{
				SpeakLine = *HeroSpeakLine;
				break;
			}
		}
	}
}

void AHeroBase::BeginPlay()
{
	BindCallbacksToDependencies();

	Super::BeginPlay();

	SpawnWeapon();

	InitHealthWidget();

	OnTakeAnyDamage.AddDynamic(this, &AHeroBase::ShowDamageNum);

	//ServerInitTeamType();

	PlayHeroSpeakLine(EHeroSpeakLineType::ET_Spawn);
}

void AHeroBase::BindCallbacksToDependencies()
{
	HealthComponent->OnHealthChanged.AddLambda([this](float NewHealth) {
		OnHealthChanged.Broadcast(NewHealth);
	});

	HealthComponent->OnMaxHealthChanged.AddLambda([this](float NewMaxHealth) {
		OnMaxHealthChanged.Broadcast(NewMaxHealth);
	});
}

void AHeroBase::Destroyed()
{
	Super::Destroyed();

	if (Weapon)
	{
		Weapon->Destroy();
	}
}

void AHeroBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHeroBase, TeamType);
	DOREPLIFETIME(AHeroBase, AimingManager);
	DOREPLIFETIME(AHeroBase, bPlayerOwned);
}

void AHeroBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		PC->SetShowMouseCursor(true);
	}

	//InitializeActorInfo();
}

void AHeroBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		PC->SetShowMouseCursor(true);
	}

	//InitializeActorInfo();
}

void AHeroBase::InitializeActorInfo()
{
	if (APlayerController* PC = GetController<APlayerController>())
	{
		if (ABrawlStarsHUD* BrawlStarsHUD = PC->GetHUD<ABrawlStarsHUD>())
		{
			if (APlayerState* PS = GetPlayerState())
			{
				BrawlStarsHUD->InitOverlay(PC, PS);
			}
		}
	}
}

void AHeroBase::EquipFlatAimingManager()
{
	UWorld* World = GetWorld();
	if (World && AimingFlatClass)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = this;
		AAimingFlat* FlatAiming = World->SpawnActor<AAimingFlat>(AimingFlatClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParam);
		AimingManager.FlatAimingManager = FlatAiming;
		AimingManager.FlatAimingManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AHeroBase::EquipLaunchAimingManager()
{
	UWorld* World = GetWorld();
	if (World && AimingLaunchClass)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = this;
		AAimingLaunch* LaunchAiming = World->SpawnActor<AAimingLaunch>(AimingLaunchClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParam);
		AimingManager.LaunchAimingManager = LaunchAiming;
		AimingManager.LaunchAimingManager->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AHeroBase::SpawnWeapon()
{
	if (WeaponClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			Weapon = World->SpawnActor<AWeaponBase>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon"));
		}
	}
}

void AHeroBase::HeroDie()
{
	SaveUltimateCurrent();

	if (ElimEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			ElimEffect,
			GetActorLocation()
		);
	}

	PlayHeroSpeakLine(EHeroSpeakLineType::ET_Die);

	Destroy();
}

void AHeroBase::SaveUltimateCurrent()
{
}

void AHeroBase::ServerInitTeamType_Implementation()
{
	DelayGetPlayerStateCallBack();
}

void AHeroBase::NormalSkillButtonOnPressed()
{
	// 物理检查
	if (SkillLockComponent->CheckPhysicsBottonNormal(true))
	{
		// 充能检查
		if (SkillLockComponent->CheckActivatableNormal())
		{
			ActiveNormalSkill();
		}
	}
}

void AHeroBase::NormalSkillButtonOnReleased()
{
	if (SkillLockComponent->CheckPhysicsBottonNormal(false))
	{
		if (SkillLockComponent->CheckReleaseableNormal())
		{
			// 释放技能
			ReleaseNormalSkill();
			// 释放技能打断回血
			HealthComponent->MultiResetRestedTime();
			// 释放技能时的台词
			PlayHeroSpeakLine(EHeroSpeakLineType::ET_NormalSkill);
		}
	}
}

void AHeroBase::UltimateSkillButtonOnPressed()
{
	// 物理检查
	if (SkillLockComponent->CheckPhysicsBottonNormal(true))
	{
		// 充能检查
		if (SkillLockComponent->CheckActivatableUltimate())
		{
			ActiveUltimateSkill();
		}
	}
}

void AHeroBase::UltimateSkillButtonOnReleased()
{
	if (SkillLockComponent->CheckPhysicsBottonNormal(false))
	{
		if (SkillLockComponent->CheckReleaseableUltimate())
		{
			// 释放技能
			ReleaseUltimateSkill();
			// 释放技能打断回血
			HealthComponent->MultiResetRestedTime();
			// 释放技能时的台词
			PlayHeroSpeakLine(EHeroSpeakLineType::ET_UltimateSkill);
		}
	}
}

void AHeroBase::GameSettingButtonOnPressed()
{

}

void AHeroBase::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHeroBase::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AHeroBase::InitHealthWidget()
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (!LevelName.Equals(TEXT("L_LoginStart")) && !LevelName.Equals(TEXT("L_LoginRoom")))
	{
		//Cast<>(HealthWidget->GetUserWidgetObject());
	}
	else
	{
		//HealthWidget->SetVisibility(false);
	}

	if (UBrawlStarsUserWidget* BSWidget = Cast<UBrawlStarsUserWidget>(HealthWidget->GetWidget()))
	{
		BSWidget->SetWidgetController(this);

		OnHealthChanged.Broadcast(HealthComponent->GetCurrentHealth());
		OnMaxHealthChanged.Broadcast(HealthComponent->GetMaxHealth());
	}
}

void AHeroBase::ShowDamageNum(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->MultiShowDamage(Damage);
	HealthComponent->MultiResetRestedTime();

	PlayHeroSpeakLine(EHeroSpeakLineType::ET_Hurt);
}

void AHeroBase::PlayHeroSpeakLine(EHeroSpeakLineType SpeakLineType)
{
	if ((IsLocallyControlled() && !IsBotControlled()) || bPlayerOwned)
	{
		switch (SpeakLineType)
		{
			case EHeroSpeakLineType::ET_NormalSkill:
			{
				PlayFSoundsWithOdds(0.2f, SpeakLine.NormalSkill);
				break;
			}
			case EHeroSpeakLineType::ET_UltimateSkill:
			{
				PlayFSoundsWithOdds(1.0f, SpeakLine.UltimateSkill);
				break;
			}
			case EHeroSpeakLineType::ET_Spawn:
			{
				PlayFSoundsWithOdds(1.0f, SpeakLine.Spawn);
				break;
			}
			case EHeroSpeakLineType::ET_Die:
			{
				PlayFSoundsWithOdds(1.0f, SpeakLine.Die);
				break;
			}
			case EHeroSpeakLineType::ET_Hurt:
			{
				PlayFSoundsWithOdds(0.2f, SpeakLine.Hurt);
				break;
			}
			case EHeroSpeakLineType::ET_Kill:
			{
				PlayFSoundsWithOdds(1.0f, SpeakLine.Kill);
				break;
			}
		}
	}
}

void AHeroBase::PlayFSoundsWithOdds(float Odds, const TMap<EHeroSpeakLineType, FSounds>& FSounds)
{
	if (FSounds.IsEmpty()) return;

	if (RandomOdds(Odds))
	{
		const auto Itr = FSounds.CreateConstIterator();
		if (Itr.Value().Sounds.IsEmpty()) return;
		USoundBase* Sound = RandomSound(Itr.Value().Sounds);
		if (Sound)
		{
			UGameplayStatics::PlaySound2D(this, Sound, 0.3f);
		}
	}
}

void AHeroBase::DelayGetPlayerStateCallBack()
{
	AGameBase* GameBase = Cast<AGameBase>(GetPlayerState());

	if (GameBase)
	{
		TeamType = GameBase->TeamType;
	}
	else
	{
		FLatentActionInfo ActionInfo;
		ActionInfo.CallbackTarget = this;
		ActionInfo.ExecutionFunction = TEXT("DelayGetPlayerStateCallBack");
		ActionInfo.Linkage = 0;
		ActionInfo.UUID = FMath::Rand();
		UKismetSystemLibrary::Delay(this, 0.5f, ActionInfo);
	}
}

void AHeroBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	//PlayerInputComponent->BindAxis("TurnRight", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("NormalSkill", IE_Pressed, this, &ThisClass::NormalSkillButtonOnPressed);
	PlayerInputComponent->BindAction("NormalSkill", IE_Released, this, &ThisClass::NormalSkillButtonOnReleased);
	PlayerInputComponent->BindAction("UltimateSkill", IE_Pressed, this, &ThisClass::UltimateSkillButtonOnPressed);
	PlayerInputComponent->BindAction("UltimateSkill", IE_Released, this, &ThisClass::UltimateSkillButtonOnReleased);
	PlayerInputComponent->BindAction("GameSetting", IE_Pressed, this, &ThisClass::GameSettingButtonOnPressed);
}