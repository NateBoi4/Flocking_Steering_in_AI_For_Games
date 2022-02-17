#include <Trackable.h>
#include "Steering.h"

class SeparationSteering : public Steering
{
public:
	SeparationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
protected:
	virtual Steering* getSteering();
};

