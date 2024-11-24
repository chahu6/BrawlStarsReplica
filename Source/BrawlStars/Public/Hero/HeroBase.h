#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DataInfo/DataInfo.h"
#include "Data/DataType.h"
#include "HeroBase.generated.h"

class AAimingFlat;
class AAimingLaunch;

USTRUCT(BlueprintType)
struct FAimingManager
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAimingFlat> FlatAimingManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAimingLaunch> LaunchAimingManager;
};

class AWeaponBase;

UCLASS()
class BRAWLSTARS_API AHeroBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroBase();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void EquipFlatAimingManager();
	void EquipLaunchAimingManager();

	// 生成武器
	void SpawnWeapon();

	// 技能相关
	virtual void ActiveNormalSkill() {};
	virtual void ReleaseNormalSkill() {};

	virtual void ActiveUltimateSkill() {};
	virtual void ReleaseUltimateSkill() {};

	// 死亡
	void HeroDie();
	void SaveUltimateCurrent();

	// 初始化团队类型
	UFUNCTION(Server, Reliable)
	void ServerInitTeamType();

	void InitializeActorInfo();

protected:	
	// 按键相关函数
	void NormalSkillButtonOnPressed();
	void NormalSkillButtonOnReleased();
	void UltimateSkillButtonOnPressed();
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
	UStaticMeshComponent* TeamDecal;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	class UHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere)
	class USkillLockComponent* SkillLockComponent;

protected:
	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimEffect;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY()
	AWeaponBase* Weapon;

	UPROPERTY(Replicated)
	FAimingManager AimingManager;

	UPROPERTY()
	FSkillMontage HeroSkills;

	UPROPERTY()
	FHeroSpeakLine SpeakLine;

	UPROPERTY(Replicated)
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
