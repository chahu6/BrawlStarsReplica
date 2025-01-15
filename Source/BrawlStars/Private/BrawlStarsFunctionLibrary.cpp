// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlStarsFunctionLibrary.h"

TArray<FRotator> UBrawlStarsFunctionLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;

	const FVector LeftOfSpred = Forward.RotateAngleAxis(-Spread / 2.f, Axis);

	const float DeltaSpred = Spread / (NumRotators + 1);
	for (int32 i = 1; i <= NumRotators; ++i)
	{
		const FVector Direction = LeftOfSpred.RotateAngleAxis(DeltaSpred * i, Axis);
		Rotators.Add(Direction.Rotation());
	}

	return Rotators;
}

TArray<FVector> UBrawlStarsFunctionLibrary::EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;

	const FVector LeftOfSpred = Forward.RotateAngleAxis(-Spread / 2.f, Axis);

	const float DeltaSpred = Spread / (NumVectors + 1);
	for (int32 i = 1; i <= NumVectors; ++i)
	{
		const FVector Direction = LeftOfSpred.RotateAngleAxis(DeltaSpred * i, Axis);
		Vectors.Add(Direction);
	}

	return Vectors;
}
