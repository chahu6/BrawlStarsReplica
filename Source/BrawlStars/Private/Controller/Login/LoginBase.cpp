#include "Controller/Login/LoginBase.h"
#include "Hero/Colt/ColtHero.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystem/GameStateSubsystem.h"

ALoginBase::ALoginBase()
{
	DefaultHeroType = AColtHero::StaticClass();

	DefaultGameModeType = EGameLevelName::EGN_GemGrab;

	SaveGameSlotName = TEXT("SaveGame");
}

void ALoginBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void ALoginBase::BeginPlay()
{
	Super::BeginPlay();
}

void ALoginBase::GetValueFromSaveGame()
{
	UGameStateSubsystem* GameStateSubsystem = GetGameInstance()->GetSubsystem<UGameStateSubsystem>();
	if (GameStateSubsystem)
	{
		FString PlayerNameOutput = GameStateSubsystem->PlayerNameInstance;
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
		if (SaveGame)
		{
			//Cast<>
		}
	}
}
