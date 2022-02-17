#pragma once

#include "GameMessage.h"
class ToggleArriveMessage : public GameMessage
{
public:
	ToggleArriveMessage(bool toggle);
	~ToggleArriveMessage();

	void process();

private:
	bool mToggle;
};

