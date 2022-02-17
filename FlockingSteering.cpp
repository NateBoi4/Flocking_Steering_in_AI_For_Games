#include <cassert>
#include <iostream>
#include <fstream>

#include "FlockingSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Steering.h"
#include "SeparationSteering.h"
#include "CohesionSteering.h"
#include "GroupAlignmentSteering.h"
#include "FaceSteering.h"

//Set values for static member variables
int FlockingSteering::mTargetBehavior = 1;
FlockingSteering::BehaviorAndWeight FlockingSteering::mBehaviors[3] = { };

FlockingSteering::FlockingSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	//Default weights
	mBehaviors[0].weight = 1.0f;
	mBehaviors[1].weight = 1.0f;
	mBehaviors[2].weight = 1.0f;
}

/*
TESTING STATIC FUNCTIONS AND MEMBER VARIABLES
*/
//void FlockingSteering::switchTarget(int target)
//{
//	mTargetBehavior = target;
//	std::cout << mTargetBehavior << std::endl;
//}
//
///*
//Ensure we are above the lowest possible value.
//Increase the value of our target weight and ensure we do not go below the minimum.
//Output a response to the console to indicate which weight changed and what its current value is
//*/
//void FlockingSteering::increaseWeights(int target)
//{
//	if (mBehaviors[target].weight < 1.0f)
//	{
//		mBehaviors[target].weight += 0.1f;
//		if (mBehaviors[target].weight > 1.0f)
//		{
//			mBehaviors[target].weight = 1.0f;
//		}
//		if (target == 0)
//		{
//			std::cout << "Separation: " << mBehaviors[target].weight << std::endl;
//		}
//		if (target == 1)
//		{
//			std::cout << "Cohesion: " << mBehaviors[target].weight << std::endl;
//		}
//		if (target == 2)
//		{
//			std::cout << "Group Alignment: " << mBehaviors[target].weight << std::endl;
//		}
//	}
//}
//
///*
//Ensure we are above the lowest possible value.
//Decrease the value of our target weight and ensure we do not go below the minimum.
//Output a response to the console to indicate which weight changed and what its current value is
//*/
//void FlockingSteering::decreaseWeights(int target)
//{
//	if (mBehaviors[target].weight > 0.0f)
//	{
//		mBehaviors[target].weight -= 0.1f;
//		if (mBehaviors[target].weight < 0.0f)
//		{
//			mBehaviors[target].weight = 0.0f;
//		}
//		if (target == 0)
//		{
//			std::cout << "Separation: " << mBehaviors[target].weight << std::endl;
//		}
//		if (target == 1)
//		{
//			std::cout << "Cohesion: " << mBehaviors[target].weight << std::endl;
//		}
//		if (target == 2)
//		{
//			std::cout << "Group Alignment: " << mBehaviors[target].weight << std::endl;
//		}
//	}
//}

//IMPLEMENT TARGET CHANGING THROUGH THE GAME FILE. USE A MEMBER VARIABLE INT AND GET THAT INT HERE TO CHANGE
//SPECIFIC BEHAVIOR. SHOULD ALLOW FOR BETTER USE OF ON SCREEN IDENTIFICATION

Steering* FlockingSteering::getSteering()
{
	//Character, target data
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//Set the behaviors of the instances
	mBehaviors[0].behavior = new SeparationSteering(mOwnerID, mTargetLoc, mTargetID);
	mBehaviors[1].behavior = new CohesionSteering(mOwnerID, mTargetLoc, mTargetID);
	mBehaviors[2].behavior = new GroupAlignmentSteering(mOwnerID, mTargetLoc, mTargetID);

	//NEED A NEW UNIT TYPE!!! OTHERWISE IT MESSES WITH BEHAVIORS

	//Accumulate all accelerations
	//for behavior in behaviors
	for (int i = 0; i < MAX_BEHAVIORS; i++)
	{
		//Run the behaviors and add their outputs to our output
		mBehaviors[i].behavior->update();
		data.acc += mBehaviors[i].behavior->getData().acc * mBehaviors[i].weight;
		//data.rotAcc += mBehaviors[i].behavior->getData().rotAcc * mBehaviors[i].weight;
		//data.rotVel += mBehaviors[i].behavior->getData().rotVel * mBehaviors[i].weight;
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
