#include <cassert>

#include "CohesionSteering.h"
#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "FaceSteering.h"

CohesionSteering::CohesionSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
{
	mType = Steering::COHESION;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* CohesionSteering::getSteering()
{
	//Character, target data
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//Direction and Distance to target
	Vector2D dir;
	float dist;

	//NULL Pointer Unit to take place of existing units for calculations
	Unit* pUnit = NULL;

	//Sum of the positions being calculated to get an average position
	Vector2D sumPositions;

	//Count of boids to use as a divisor for average calculation. Better as a float.
	float count = 0.0f;

	//Threshold to take action
	float threshold = 150.0f;

	//Go through list of targets and check if we are within the distance to take action
	for (auto i = gpGame->getUnitManager()->getNumBoids(); i > 0; i--)
	{
		pUnit = gpGame->getUnitManager()->getUnit(i);
		if (pUnit != NULL && pUnit != pOwner)
		{
			dir = pUnit->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			dist = dir.getLength();
			if (dist < threshold)
			{
				//Add the unit's position to the sum of positions and increment the divisor
				sumPositions += pUnit->getPositionComponent()->getPosition();
				count++;
			}
		}
	}

	//If no one is nearby, return nothing
	if (count == 0.0f)
	{
		return NULL;
	}

	//Get the average position of our boids
	Vector2D averagePosition = sumPositions / count;

	//Calculate the direction to move toward the average position
	dir = averagePosition - pOwner->getPositionComponent()->getPosition();

	//Normalize and multiply by maximum acceleration
	dir.normalize();
	dir *= pOwner->getMaxAcc();

	//Set the accelerations and return the steering.
	data.acc = dir;
	//data.rotVel = 0.0f;
	//data.rotAcc = 0.0f;

	this->mData = data;
	return this;
}
