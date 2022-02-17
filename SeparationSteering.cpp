#include <cassert>
#include <algorithm>

#include "SeparationSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Steering.h"

SeparationSteering::SeparationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
{
	mType = Steering::SEPARATION;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* SeparationSteering::getSteering()
{
	//Character, target data
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//Threshold to take action
	float threshold = 150.0f;

	//Constant coefficient of decay for the inverse law force
	float decayCoefficient = 100000.0f;

	//NULL Pointer Unit to take place of existing units for calculations
	Unit* pUnit = NULL;

	//Direction and distance to targets
	Vector2D dir;
	float dist;

	//Sum of the accelerations being calculated to get a direction of travel
	Vector2D sumForces;

	//Go through list of targets, excluding self, and get the vector away from each target
	for (auto i = gpGame->getUnitManager()->getNumBoids(); i > 0; i--)
	{
		if (i != mOwnerID)
		{
			pUnit = gpGame->getUnitManager()->getUnit(i);
			if (pUnit != NULL)
			{
				dir = pOwner->getPositionComponent()->getPosition() - pUnit->getPositionComponent()->getPosition();
				dist = dir.getLength();
				//Check if we are within the threshold
				if (dist < threshold)
				{
					//Calculate strength of repulsion
					float decay = decayCoefficient / (dist * dist);
					float strength = min(decay, pOwner->getMaxAcc());

					//Add the acceleration
					dir.normalize();
					sumForces += dir * strength;
				}
			}
		}
	}

	//Set the accelerations and return the steering
	data.acc = sumForces;
	//data.rotVel = 0.0f;
	//data.rotAcc = 0.0f;

	this->mData = data;
	return this;
}
