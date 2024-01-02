// Fill out your copyright notice in the Description page of Project Settings.


#include "FlatSkillOnly.h"

void AFlatSkillOnly::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AFlatSkillOnly::BeginPlay()
{
	Super::BeginPlay();

	EquipFlatAimingManager();
}
