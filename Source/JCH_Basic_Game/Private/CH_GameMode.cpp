#include "CH_GameMode.h"
#include "CH_Character.h"
#include "CH_PlayerController.h"
#include "CH_GameState.h"

ACH_GameMode::ACH_GameMode()
{
	DefaultPawnClass = ACH_Character::StaticClass();
	PlayerControllerClass = ACH_PlayerController::StaticClass();

	GameStateClass = ACH_GameState::StaticClass();
}