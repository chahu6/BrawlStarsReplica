#include "GameModes/BrawlStarsGameMode.h"
#include "Controller/Game/GameBaseController.h"
#include "Kismet/GameplayStatics.h"

void ABrawlStarsGameMode::OnPostLogin(AController* NewPlayer)
{
	//Cast<AGameBaseController>(NewPlayer);
	//PlayerInputDisable();
}

void ABrawlStarsGameMode::PlayerInputDisable()
{
	int32 PlayerNumber = GetNumPlayers();
	for (int i = 0; i < PlayerNumber; ++i)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, i);
		AGameBaseController* GameBaseControl = Cast<AGameBaseController>(PlayerController);
		if (GameBaseControl)
		{
			GameBaseControl->PlayerControllerInputDisable();
		}
	}
}
