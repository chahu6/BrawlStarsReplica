#include "Controller/GameBaseController.h"

void AGameBaseController::BeginPlay()
{
	Super::BeginPlay();

	//PlayerControllerInputDisable();
}

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

void AGameBaseController::GameIntroduction()
{

}
