#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DataInfo/DataInfo.h"
#include "Data/DataType.h"
#include "HeroBase.generated.h"

class AAimingFlat;
class AAimingLaunch;
class AWeaponBase;
class UHealthComponent;
class USkillLockComponent;
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;

USTRUCT(BlueprintType)
struct FAimingManager
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAimingFlat> FlatAimingManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAimingLaunch> LaunchAimingManager;
};

UCLASS()
class BRAWLSTARS_API AHeroBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroBase();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 死亡
	void HeroDie();
	void SaveUltimateCurrent();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipFlatAimingManager();
	void EquipLaunchAimingManager();

	// 生成武器
	void SpawnWeapon();

	// 技能相关
	virtual void ActiveNormalSkill() {};
	virtual void ReleaseNormalSkill() {};

	virtual void ActiveUltimateSkill() {};
	virtual void ReleaseUltimateSkill() {};

	// 初始化团队类型
	UFUNCTION(Server, Reliable)
	void ServerInitTeamType();

protected:	
	// 按键相关函数
	void NormalSkillButtonOnPressed();
	void NormalSkillButtonOnReleased();
	void UltimateSkillButtonOnPressed();
	void UltimateSkillButtonOnReleased();
	void GameSettingButtonOnPressed();

	void MoveForward(float Value);
	void MoveRight(float Value);

private:
	// 初始化Widget
	void InitHealthWidget();

	// 显示伤害数字
	UFUNCTION()
	void ShowDamageNum(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// 播放英雄台词
	void PlayHeroSpeakLine(EHeroSpeakLineType SpeakLineType);

	// 播放声音
	void PlayFSoundsWithOdds(float Odds, const TMap<EHeroSpeakLineType, FSounds>& FSounds);

	UFUNCTION()
	void DelayGetPlayerStateCallBack();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TeamDecal;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkillLockComponent> SkillLockComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthWidget;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ElimEffect;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<AAimingFlat> AimingFlatClass;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<AAimingLaunch> AimingLaunchClass;

	UPROPERTY()
	TObjectPtr<AWeaponBase> Weapon;

	UPROPERTY(BlueprintReadOnly, Replicated)
	FAimingManager AimingManager;

	UPROPERTY(BlueprintReadOnly)
	FSkillMontage HeroSkillMontage;

	UPROPERTY()
	FHeroSpeakLine SpeakLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	ETeamType TeamType;

	UPROPERTY(Replicated)
	bool bPlayerOwned;

public:
	FORCEINLINE bool RandomOdds(float Odds) const { return FMath::TruncToFloat(Odds * 100) > FMath::RandRange(0, 100); }
	FORCEINLINE USoundBase* RandomSound(const TArray<USoundBase*>& Sounds) const { return Sounds[FMath::RandHelper(Sounds.Num()-1)]; }
	FORCEINLINE AWeaponBase* GetWeapon() { return Weapon; }
	FORCEINLINE USkillLockComponent* GetSkillLockComponent() const { return SkillLockComponent; }
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
};
