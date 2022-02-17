#include <cassert>

#include "GroupAlignmentSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Steering.h"
#include "FaceSteering.h"

GroupAlignmentSteering::GroupAlignmentSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
{
	mType = Steering::GROUP_ALIGNMENT;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* GroupAlignmentSteering::getSteering()
{
	//Character, target data
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//Direction and Distance to the target
	Vector2D dir;
	float dist;

	//NULL Pointer Unit to take place of existing units for calculations
	Unit* pUnit = NULL;

	//Sum of the Velocities being calculated to get an average velocity
	Vector2D sumVelocities;

	//Threshold to take action
	float threshold = 150.0f;

	//Count of boids to use as a divisor for average calculation. Better as a float.
	float count = 0.0f;

	//MAKE THIS A FUNCTION IN UNIT_MANAGER

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
				//Add the unit's velocity to the sum of velocities and increment the divisor
				sumVelocities += pUnit->getPhysicsComponent()->getVelocity();
				count++;
			}
		}
	}

	//If no one is nearby, return nothing
	if (count == 0.0f)
	{
		return NULL;
	}

	//Get the average velocity of our boids
	Vector2D averageVelocity = sumVelocities / count;

	//Calculate the direction of the average velocities
	dir = averageVelocity - pOwner->getPhysicsComponent()->getVelocity();

	//Normalize and multiply by maximum acceleration
	dir.normalize();
	dir *= pOwner->getMaxAcc();

	//dir += pOwner->getPositionComponent()->getPosition();

	//Make units face the direction of the average velocity
	//FaceSteering face(mOwnerID, dir);
	//face.update();
	//PhysicsData faceData = face.getData();

	//Set the accelerations and return the steering.
	data.acc = dir;
	//data.rotAcc = faceData.rotAcc;
	//data.rotVel = faceData.rotVel;

	this->mData = data;
	return this;
}

//ALTERNATE IMPLEMENTATION
/*
* Vector2D dir
* int count
for(int i = 1; i < gpGame->getUnitManager()->getNumUnits(); i++
{
	if(i != mOwnerID)
	{
		if((gpGame->getUnitManager()->getUnit(i)->getPositionComponenet()->getPosition() - pOwner->getPositionComponent()->getPosition()) < 300.0f)
		{
			dir.x += data.vel.x
			dir.y += data.vel.y
			count++
		}
	}
}

if(count == 0)
{
	return nullptr
}

dir.x /= count
dir.y /= count
dir.normalize()
data.acc = dir

this->mData = data;
return this;
*/
