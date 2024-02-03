#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo.generated.h"

class AHeroBase;
class AWeaponBase;
class ASkillBase;
class ABulletBase;

UENUM(BlueprintType)
enum class EHeroSpeakLineType : uint8
{
	None				UMETA(DisplayName = "None"),
	ET_NormalSkill		UMETA(DisplayName = "NormalSkill"),
	ET_UltimateSkill	UMETA(DisplayName = "UltimateSkill"),
	ET_Spawn			UMETA(DisplayName = "Spawn"),
	ET_Die				UMETA(DisplayName = "Die"),
	ET_Hurt				UMETA(DisplayName = "Hurt"),
	ET_Kill				UMETA(DisplayName = "Kill"),
};

UENUM(BlueprintType)
enum class ESkillClassification : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	
	ESC_Normal UMETA(DisplayName = "Normal"),
	ESC_Ultimate UMETA(DisplayName = "Ultimate")
};

USTRUCT(BlueprintType)
struct FHeroAttribute : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<AHeroBase> HeroType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<AWeaponBase> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FString ChineseName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float DefaultHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float MoveSpeed;
};

USTRUCT(BlueprintType)
struct FSkillAttribute : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<AHeroBase> HeroType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<ASkillBase> SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	ESkillClassification SkillClassification;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	int32 SkillProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float SkillReleaseAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float SkillRechage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<ABulletBase> BulletType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float BulletAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float BulletDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float BulletMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float BulletLifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	bool bBulletSenceDestructible;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	bool bBuletIsLongLive;
};

USTRUCT(BlueprintType)
struct FSkillMontage : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<AHeroBase> HeroType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float NormalReleaseOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	UAnimMontage* NormalMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FName NormalMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<ASkillBase> NormalSkillType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	float UltimateReleaseOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	UAnimMontage* UltimateMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FName UltimateMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<ASkillBase> UltimateSkillType;
};

USTRUCT(BlueprintType)
struct FSounds
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<USoundBase*> Sounds;
};

USTRUCT(BlueprintType)
struct FHeroSpeakLine : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TSubclassOf<AHeroBase> HeroType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TMap<EHeroSpeakLineType, FSounds> NormalSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TMap<EHeroSpeakLineType, FSounds> UltimateSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TMap<EHeroSpeakLineType, FSounds> Spawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TMap<EHeroSpeakLineType, FSounds> Die;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TMap<EHeroSpeakLineType, FSounds> Hurt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	TMap<EHeroSpeakLineType, FSounds> Kill;
};

USTRUCT(BlueprintType)
struct FAimInfo
{
	GENERATED_USTRUCT_BODY()

	bool bIsFlatAiming;

	bool bIsLaunchAming;

	bool bIsViewportCentrePointSet;

	FVector2D ViewportCentrePoint;

	FVector2D RealViewportCenterMousePoint;

	float AimDistance;

	float AimRotationYaw;
};