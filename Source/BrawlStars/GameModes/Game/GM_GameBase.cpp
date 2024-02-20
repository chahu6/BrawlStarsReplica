#include "GM_GameBase.h"
#include "BrawlStars/Controller/Game/GameBaseController.h"

void AGM_GameBase::OnPostLogin(AController* NewPlayer)
{
	Cast<AGameBaseController>(NewPlayer);
}
