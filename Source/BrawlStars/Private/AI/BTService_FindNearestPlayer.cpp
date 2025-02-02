// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Hero/HeroBase.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AHeroBase* OwningHero = Cast<AHeroBase>(AIOwner->GetPawn());
	if (IsValid(OwningHero))
	{
		TArray<AActor*> Actors;
		TArray<AActor*> Enemies;
		UGameplayStatics::GetAllActorsOfClass(OwningHero, AHeroBase::StaticClass(), Actors);
		Enemies = Actors.FilterByPredicate([OwningHero](AActor* Elem) {
			AHeroBase* HeroBase = Cast<AHeroBase>(Elem);
			return OwningHero->GetTeamType() != HeroBase->GetTeamType();
		});

		float ClosestDistance = TNumericLimits<float>::Max();
		AActor* ClosestEnemy = nullptr;
		for (AActor* Enemy : Enemies)
		{
			if (IsValid(Enemy))
			{
				const float Distance = OwningHero->GetDistanceTo(Enemy);
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestEnemy = Enemy;
				}
			}
		}
		UBTFunctionLibrary::SetBlackboardValueAsObject(this, EnemySelector, ClosestEnemy);
		UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
	}
}
