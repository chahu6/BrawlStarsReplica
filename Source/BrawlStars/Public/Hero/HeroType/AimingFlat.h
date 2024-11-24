#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../DataInfo/DataInfo.h"
#include "Utils/DoOnce.h"
#include "AimingFlat.generated.h"

class AHeroBase;
class UGetActorScreenPointComponent;

UCLASS()
class BRAWLSTARS_API AAimingFlat : public AActor
{
	GENERATED_BODY()
	
	friend class AFlatSkillOnly;
public:	
	AAimingFlat();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void Tick(float DeltaTime) override;

	void CenterMousePosition();

	void SkillMontageStop();

	void SetHeroSkillReleaseRotation(float AddModify);

	void LockMovementOrientRotation();

	void UnlockMovementOrientRotation();

	void PlayMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	void ReleaseFlatSkill(const TSubclassOf<ASkillBase>& SkillType);

protected:
	void InitAimingScreenPoint();

	bool NotAIControlled();

private:
	void FlatAimingManager();

	void UpdateDecalShap();

	void UpdateAimDistanceAndRotation();

	void InitActorScreenPoint();

	UFUNCTION()
	void ViewportFinish();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDecalComponent> AimingDecal;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGetActorScreenPointComponent> GetActorScreenPointComponent;

	UPROPERTY(EditAnywhere, Category = "Decal")
	TObjectPtr<UMaterialInterface> Decal_Rectangle;

	UPROPERTY(EditAnywhere, Category = "Decal")
	TObjectPtr<UMaterialInterface> Decal_Sector;

	UPROPERTY(EditAnywhere, Category = "Decal")
	TObjectPtr<UMaterialParameterCollection> Aim_Decal;

	UPROPERTY()
	TObjectPtr<AHeroBase> Hero;

	UPROPERTY()
	FAimInfo AimingInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalAngleNormal;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalAngleUltimate;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalDistanceNormal;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float DecalDistanceUltimate;

private:
	FDoOnce DoOnce;
};
