#include "Hero/HeroType/AimingLaunch.h"
#include "Hero/HeroBase.h"
#include "Components/SplineComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AAimingLaunch::AAimingLaunch()
{
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;

	LaunchSpline = CreateDefaultSubobject<USplineComponent>(TEXT("LaunchSpline"));
	LaunchSpline->SetupAttachment(RootComponent);
	//LaunchSpline->SetAbsolute();

	LaunchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

void AAimingLaunch::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat OnTimelineFloatEvent;
	FOnTimelineEvent OnTimelineFinishedEvent;
	OnTimelineFloatEvent.BindUFunction(this, TEXT("OnTimelineTick"));
	OnTimelineFinishedEvent.BindUFunction(this, TEXT("OnTimelineEndEvent"));

	ensure(Curver);

	LaunchTimeline->AddInterpFloat(Curver, OnTimelineFloatEvent);
	LaunchTimeline->SetTimelineLength(ETimelineLengthMode::TL_LastKeyFrame);
	LaunchTimeline->SetTimelineFinishedFunc(OnTimelineFinishedEvent);
}

void AAimingLaunch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LaunchAimManager();
}

void AAimingLaunch::ReleaseLaunchSkill(bool bInIsHeroLaunch, AActor* InActorLaunch)
{
	bIsHeroLaunch = bInIsHeroLaunch;
	ActorLaunch = InActorLaunch;

	bStopLaunch = false;

	LaunchTimeline->SetPlayRate(LaunchDistanceInfo.LaunchVelocityMax / LaunchDistanceInfo.LaunchVelocity.Length());

	if (!LaunchPath.LaunchPathPositions.IsEmpty())
	{
		LaunchTimeline->PlayFromStart();
	}
	else
	{
		FinishedSkill();
	}
}

void AAimingLaunch::FinishedSkill()
{
	OnLaunchEndDelegate.Broadcast();
	bIsHeroLaunch = false;
	LaunchPath.LaunchPathPositions.Empty();
}

void AAimingLaunch::LaunchAimManager()
{
	if (IsValid(Hero) && Hero->IsLocallyControlled() && AimingInfo.bIsLaunchAming)
	{
		UpdateAimDistanceAndRotation();

		if (AimingInfo.AimDistance != 0.f)
		{
			UpdateLaunchAimInfo();

			if (NotAIControlled())
			{
				AimingDecal->SetVisibility(true);
			}
			ShowLaunchAim();
		}

		bDoOnce = true;
	}
	else
	{
		if (bDoOnce)
		{
			bDoOnce = false;

			AimingDecal->SetVisibility(false);
			AimingInfo.AimDistance = 0.f;
			LaunchDistanceInfo.LaunchDistance = 0.f;
			for (USplineMeshComponent* SplineMesh : LaunchPath.LaunchPathComponents)
			{
				SplineMesh->DestroyComponent();
			}
			LaunchPath.LaunchPathComponents.Empty();
		}
	}
}

void AAimingLaunch::UpdateLaunchAimInfo()
{
	const float AimDistance = AimingInfo.AimDistance;
	float AimDistanceDelta = AimDistance - LaunchDistanceInfo.AimDistanceOld;

	// Make Sure Distance Back to Zero
	if (AimDistanceDelta <= 0.f)
	{
		AimDistanceDelta *= 5;
	}

	const float ToTarget = FMath::Clamp(LaunchDistanceInfo.LaunchDistance + AimDistanceDelta, 0, LaunchDistanceInfo.AimDistanceMax);
	const float Distance = FMath::FInterpTo(LaunchDistanceInfo.LaunchDistance, ToTarget, GetWorld()->GetDeltaSeconds(), 3.f);

	FVector LaunchVelocity = GetLaunchVelocity(Distance, LaunchDistanceInfo.AimDistanceMax);

	LaunchDistanceInfo.AimDistanceOld = AimDistance;
	LaunchDistanceInfo.LaunchDistance = Distance;
	LaunchDistanceInfo.LaunchVelocity = LaunchVelocity.GetClampedToSize(0.f, LaunchDistanceInfo.LaunchVelocityMax);
}

void AAimingLaunch::ShowLaunchAim()
{
	const FRotator SplineRotation = LaunchSpline->GetComponentRotation();
	LaunchSpline->SetWorldRotation(FRotator(SplineRotation.Pitch, AimingInfo.AimRotationYaw, SplineRotation.Roll));
	LaunchPath.LaunchPathComponents.ContainsByPredicate([](USplineMeshComponent* SplineMesh) {
		SplineMesh->DestroyComponent();
		return false;
	});
	LaunchPath.LaunchPathComponents.Empty();
	LaunchSpline->ClearSplinePoints();

	//UGameplayStatics::GetAllActorsOfClass(this, );
	FHitResult HitResult;
	TArray<FVector> OutPathPositions;
	FVector OutLastTraceDestination;
	UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(this,
		HitResult,
		OutPathPositions,
		OutLastTraceDestination,
		Hero->GetMesh()->GetSocketLocation(TEXT("LaunchStartPoint")),
		LaunchDistanceInfo.LaunchVelocity,
		true, 10.f,
		{ EObjectTypeQuery::ObjectTypeQuery1 }, false,
		{}, EDrawDebugTrace::ForOneFrame, 5.f, 15.f, 5.f);

	LaunchPath.LaunchPathPositions = OutPathPositions;
	if (NotAIControlled())
	{
		AimingDecal->SetWorldLocation(OutLastTraceDestination);
		for (int32 i = 0; i < OutPathPositions.Num(); ++i)
		{
			LaunchSpline->AddSplinePointAtIndex(OutPathPositions[i], i, ESplineCoordinateSpace::World, true);
		}
		LaunchSpline->SetSplinePointType(OutPathPositions.Num() - 1, ESplinePointType::CurveClamped);
		AddLaunchPathComponent(0.5f);
	}
}

void AAimingLaunch::AddLaunchPathComponent(float Scale)
{
	const int32 Num = LaunchSpline->GetNumberOfSplinePoints() - 1;
	for (int32 i = 0; i < Num; ++i)
	{
		USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), true, FTransform(), false));
		SplineMesh->SetStartScale(FVector2D(Scale));
		SplineMesh->SetEndScale(FVector2D(Scale));
		SplineMesh->SetStartAndEnd(LaunchPath.LaunchPathPositions[i], LaunchSpline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local),
			LaunchPath.LaunchPathPositions[i + 1], LaunchSpline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local));

		LaunchPath.LaunchPathComponents.Add(SplineMesh);
	}
}

FVector AAimingLaunch::GetLaunchVelocity(float InLaunchDistance, float LaunchMax)
{
	const FRotator Rotation = LaunchSpline->GetComponentRotation();
	const FVector Forward = Rotation.Vector();
	const FVector UpVector = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z);

	const FVector Direction = (Forward + UpVector * 0.8f).GetSafeNormal();

	FVector LaunchVelocity = (Direction + Direction * InLaunchDistance) * 10.f;

	LaunchVelocity = LaunchVelocity.GetClampedToSize(0, LaunchMax);

	if (GetInstigator()->IsBotControlled())
	{
		if (FPlatformMisc::IsPackagedForDistribution())
		{
			return LaunchVelocity * 6;
		}
		else
		{
			return LaunchVelocity * 2;
		}
	}
	else
	{
		if (FPlatformMisc::IsPackagedForDistribution())
		{
			return LaunchVelocity * 8;
		}
		else
		{
			return LaunchVelocity * 3;
		}
	}
}

void AAimingLaunch::OnTimelineTick(float Value)
{
	if (GetLaunchActor())
	{
		if (Value < 0.95f && !bStopLaunch)
		{
			if (IsValid(LaunchComponent))
			{
				const float PathNumber = LaunchPath.LaunchPathPositions.Num();
				const int32 Index = FMath::TruncToInt32(FMath::Lerp(0.f, PathNumber, Value));
				if (bIsHeroLaunch)
				{
					LaunchComponent->SetWorldLocation(LaunchPath.LaunchPathPositions[Index], true);
				}
				else
				{
					const FVector DeltaLocation = FMath::VInterpTo(
						LaunchComponent->GetComponentLocation(),
						LaunchPath.LaunchPathPositions[Index], 
						GetWorld()->GetDeltaSeconds(),
						4.f);

					LaunchComponent->SetWorldLocation(DeltaLocation, true);
				}
			}
		}
		else
		{
			LaunchTimeline->Stop();
			FinishedSkill();
		}
	}
}

bool AAimingLaunch::GetLaunchActor()
{
	if (bIsHeroLaunch)
	{
		if (IsValid(Hero))
		{
			LaunchComponent = Hero->GetCapsuleComponent();
			return true;
		}
	}
	else
	{
		if (IsValid(ActorLaunch))
		{
			LaunchComponent = ActorLaunch->GetRootComponent();
			return true;
		}
	}
	return false;
}

void AAimingLaunch::OnTimelineEndEvent()
{
	FinishedSkill();
}
