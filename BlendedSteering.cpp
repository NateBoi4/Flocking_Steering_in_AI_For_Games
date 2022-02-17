#include "BlendedSteering.h"
#include "Game.h"
#include "Unit.h"
#include "UnitManager.h"
#include "ObstacleAvoidance.h"
#include "ArriveAndFaceSteering.h"
#include "FaceSteering.h"
#include "FlockingSteering.h"

#include <cassert>

//Set static member variable values
int BlendedSteering::mTargetBehavior = 1;
BlendedSteering::BehaviorAndWeight BlendedSteering::mBehaviors[3] = { };

BlendedSteering::BlendedSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	//Default weights
	mBehaviors[0].weight = 0.0f;
	mBehaviors[1].weight = 5.0f;
	mBehaviors[2].weight = 1.0f;
}

void BlendedSteering::toggleArriveSteering(bool toggle) 
{
	if (toggle) 
	{
		mBehaviors[0].weight = 1.0f;
	}
	else if (!toggle) 
	{
		mBehaviors[0].weight = 0.0f;
	}
}

Steering* BlendedSteering::getSteering()
{
	//Character, target data
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//Set the behaviors of the instances
	mBehaviors[0].behavior = new ArriveAndFaceSteering(mOwnerID, mTargetLoc, mTargetID);
	mBehaviors[1].behavior = new ObstacleAvoidance(mOwnerID, mTargetLoc, mTargetID);
	mBehaviors[2].behavior = new FlockingSteering(mOwnerID, mTargetLoc, mTargetID);

	//Accumulate all accelerations
	//for behavior in behaviors
	for (int i = 0; i < MAX_BEHAVIORS; i++)
	{
		//Run the behaviors and add their outputs to our output
		mBehaviors[i].behavior->update();
		data.acc += mBehaviors[i].behavior->getData().acc * mBehaviors[i].weight;
	}

	FaceSteering face(mOwnerID, pOwner->getPositionComponent()->getPosition() + data.vel);
	face.update();
	PhysicsData faceData = face.getData();

	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;

	this->mData = data;

	//Delete the created behaviors to avoid memory leaks
	for (int i = 0; i < MAX_BEHAVIORS; i++)
	{
		delete mBehaviors[i].behavior;
	}

	return this;
}
