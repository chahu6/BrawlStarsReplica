#include "GM_GameBase.h"
#include "BrawlStars/Controller/Game/GameBaseController.h"
#include "Kismet/GameplayStatics.h"

void AGM_GameBase::OnPostLogin(AController* NewPlayer)
{
	//Cast<AGameBaseController>(NewPlayer);
	//PlayerInputDisable();
}

void AGM_GameBase::PlayerInputDisable()
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
