#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

class AHeroBase;

UCLASS()
class BRAWLSTARS_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletBase();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;

private:
	UFUNCTION()
	void OnCubeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool CheckTeamate(AActor* OtherActor);

	void HitEnemy();

	void RechageUltimate();

	void ApplyDamageAndDestroy();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess ="true"))
	class UBoxComponent* Cube;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectilMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BulletHead;

	UPROPERTY(EditAnywhere)
	USoundBase* BulletSoundEffect;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* BulletSoundAttenuation;

private:
	UPROPERTY()
	float Damage;

	UPROPERTY()
	bool bIsLongLive;

	UPROPERTY()
	bool bSenceDestructible;

	UPROPERTY()
	float BulletSoundVolume;

	UPROPERTY()
	AHeroBase* Enemy;
};
