#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DataInfo/DataInfo.h"
#include "../../Data/DataType.h"
#include "HeroBase.generated.h"

USTRUCT(BlueprintType)
struct FAimingManager
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AAimingFlat* FlatAimingManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AAimingLaunch* LaunchAimingManager;
};

class AWeaponBase;

UCLASS()
class BRAWLSTARS_API AHeroBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroBase();

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

	// ��������
	void SpawnWeapon();

	// �������
	virtual void ActiveNormalSkill() {};
	virtual void ReleaseNormalSkill() {};

	virtual void ActiveUltimateSkill() {};
	virtual void ReleaseUltimateSkill() {};

	// ����
	void HeroDie();
	void SaveUltimateCurrent();

	// ��ʼ���Ŷ�����
	UFUNCTION(Server, Reliable)
	void ServerInitTeamType();

protected:	
	// ������غ���
	void NormalSkillButtonOnPressed();
	void NormalSkillButtonOnReleased();
	void UltimateSkillButtonOnPressed();
	void GameSettingButtonOnPressed();

	void MoveForward(float Value);
	void MoveRight(float Value);

private:
	// ��ʼ��Widget
	void InitHealthWidget();

	// ��ʾ�˺�����
	void ShowDamageNum(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// ����Ӣ��̨��
	void PlayHeroSpeakLine(EHeroSpeakLineType SpeakLineType);

	// ��������
	void PlayFSoundsWithOdds(float Odds, const TMap<EHeroSpeakLineType, FSounds>& FSounds);

	UFUNCTION()
	void DelayGetPlayerStateCallBack();

private:
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthWidget;

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
	FORCEINLINE USoundBase* RandomSound(const TArray<USoundBase*>& Sounds) const { return Sounds[FMath::RandHelper(Sounds.Num())]; }
	FORCEINLINE AWeaponBase* GetWeapon() { return Weapon; }
	FORCEINLINE USkillLockComponent* GetSkillLockComponent() const { return SkillLockComponent; }
};