#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hero/DataInfo/DataInfo.h"
#include "Utils/DoOnce.h"
#include "AimingFlat.generated.h"

class AHeroBase;
class UGetActorScreenPointComponent;

UCLASS()
class BRAWLSTARS_API AAimingFlat : public AActor
{
	GENERATED_BODY()
	
public:	
	AAimingFlat();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	void CenterMousePosition();

	UFUNCTION(BlueprintCallable)
	void SkillMontageStop();

	UFUNCTION(BlueprintCallable)
	void SetHeroSkillReleaseRotation(float AddModify);

	UFUNCTION(BlueprintCallable)
	void LockMovementOrientRotation();

	UFUNCTION(BlueprintCallable)
	void UnlockMovementOrientRotation();

	void PlayMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);

	void ReleaseFlatSkill(const TSubclassOf<ASkillBase>& InSkillClass);

	UPROPERTY()
	FAimInfo AimingInfo;

protected:
	UFUNCTION()
	void InitAimingScreenPoint(APawn* Pawn, AController* OldController, AController* NewController);

	bool NotAIControlled();

	void FlatAimingManager();

	void UpdateDecalShap();

	void UpdateAimDistanceAndRotation();

	void InitActorScreenPoint();

	UFUNCTION()
	void ViewportFinish();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
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
