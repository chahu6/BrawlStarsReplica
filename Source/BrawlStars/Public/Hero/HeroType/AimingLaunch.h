#pragma once

#include "CoreMinimal.h"
#include "Hero/HeroType/AimingFlat.h"
#include "AimingLaunch.generated.h"

class USplineComponent;
class UTimelineComponent;
class USplineMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchEndSignature);

USTRUCT(BlueprintType)
struct FLaunchPath
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> LaunchPathPositions;

	UPROPERTY(BlueprintReadOnly)
	TArray<USplineMeshComponent*> LaunchPathComponents;
};

UCLASS()
class BRAWLSTARS_API AAimingLaunch : public AAimingFlat
{
	GENERATED_BODY()
	
public:	
	AAimingLaunch();
	virtual void OnConstruction(const FTransform& Transform) override {};
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "AimingLaunch")
	FOnLaunchEndSignature OnLaunchEndDelegate;

protected:
	UFUNCTION(BlueprintCallable)
	void ReleaseLaunchSkill(bool bInIsHeroLaunch, AActor* InActorLaunch);

	void FinishedSkill();

	void LaunchAimManager();

	void ShowLaunchAim();

	void UpdateLaunchAimInfo();

	void AddLaunchPathComponent(float Scale);

	FVector GetLaunchVelocity(float InLaunchDistance, float LaunchMax);

	UFUNCTION()
	void OnTimelineTick(float Value);	// Timeline 更新时调用

	UFUNCTION()
	void OnTimelineEndEvent(); // Timeline 完成最后一帧后调用

	bool GetLaunchActor();

protected:
	UPROPERTY()
	FLaunchDistance LaunchDistanceInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USplineComponent> LaunchSpline;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHeroLaunch;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ActorLaunch;

	UPROPERTY(BlueprintReadWrite)
	bool bStopLaunch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTimelineComponent> LaunchTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> Curver;

	UPROPERTY(BlueprintReadOnly)
	FLaunchPath LaunchPath;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USceneComponent> LaunchComponent;

	bool bDoOnce = true;
};
