#include "ToggleArriveMessage.h"
#include "Game.h"
#include "GameMessageManager.h"
#include "BlendedSteering.h"

ToggleArriveMessage::ToggleArriveMessage(bool toggle)
	: GameMessage(TOGGLE_ARRIVE_MESSAGE)
	, mToggle(toggle)
{

}

ToggleArriveMessage::~ToggleArriveMessage()
{
}

void ToggleArriveMessage::process()
{
	BlendedSteering::toggleArriveSteering(mToggle);
}
