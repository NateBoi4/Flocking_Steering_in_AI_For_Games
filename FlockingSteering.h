#include <Trackable.h>
#include "Steering.h"
#include <iostream>

class FlockingSteering : public Steering
{
public:
	FlockingSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	/*static void switchTarget(int target);
	static void increaseWeights(int target);
	static void decreaseWeights(int target);*/

	struct BehaviorAndWeight
	{
		Steering* behavior;
		float weight;
	};

protected:
	virtual Steering* getSteering();

private:
	static int mTargetBehavior;

	static const int MAX_BEHAVIORS = 3;

	static BehaviorAndWeight mBehaviors[MAX_BEHAVIORS];

};

