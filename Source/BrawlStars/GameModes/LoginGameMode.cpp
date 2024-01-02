// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"

ALoginGameMode::ALoginGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/CoreUObject.Class'/Script/BrawlStars.ColtHero'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
