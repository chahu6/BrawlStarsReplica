#include "BulletBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABulletBase::ABulletBase()
{
 	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Cube = CreateDefaultSubobject<UBoxComponent>(TEXT("Cube"));
	RootComponent = Cube;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);

	BulletHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletHead"));
	BulletHead->SetupAttachment(RootComponent);

	ProjectilMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectilMovement"));

}

void ABulletBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

