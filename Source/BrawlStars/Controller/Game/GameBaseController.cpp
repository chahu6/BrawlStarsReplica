#include "GameBaseController.h"

void AGameBaseController::PlayerControllerInputDisable()
{
	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn))
	{
		ControlledPawn->DisableInput(this);
	}
}

void AGameBaseController::PlayerControllerInputEnable()
{
	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn))
	{
		ControlledPawn->EnableInput(this);
	}
}
